#include "ZomboContentBlock.h"
#include "loaders/LoaderJsonAtlas.h"
#include <cctype>
#include "../ZomboLog.h"
#include "../../fonts/FontsManager.h"

namespace zombo
{
	ZomboContentBlock::ZomboContentBlock() : _loaded(false)
	{
		
	}

	ZomboContentBlock::~ZomboContentBlock()
	{
	}

	void ZomboContentBlock::enqueueResource(const char* fileName)
	{
		_loadingQueue.push(fileName);
	}

	bool ZomboContentBlock::isLoaded() const
	{
		return _loaded;
	}

	ZomboContentFrameAnimation* ZomboContentBlock::createAnimation(const char* fileName) const
	{
		int leadingZeros = 0;
		if (hasFormat(fileName, "{n:000}"))
		{
			leadingZeros = 3;
		}
		else if (hasFormat(fileName, "{n:00}"))
		{
			leadingZeros = 2;
		}
		ZomboContentFrameAnimation *animation = ZomboContentFrameAnimation::create();
		for (int i = 0; i < 100; i++) // todo: make constant for max frames count
		{
			std::string imageName = format(fileName, i, leadingZeros);
			ZomboContentImage* image = getImage(imageName.c_str());
			if (image == nullptr)
				break;
			animation->frames.push_back(image);
		}
		return animation;
	}

	ZomboContentImage* ZomboContentBlock::getImage(const char* fileName) const
	{
		return getMapValue(_images, fileName);
	}

	ZomboContentTexture* ZomboContentBlock::getTexture(const char* fileName) const
	{
		return getMapValue(_textures, fileName);
	}

	ZomboContentAtlasPage* ZomboContentBlock::getAtlasPage(const char* fileName) const
	{
		return getMapValue(_altasPages, fileName);
	}

	ZomboContentAtlas* ZomboContentBlock::getAtlas(const char* fileName) const
	{
		return getMapValue(_altases, fileName);
	}

	ZomboBitmapFont* ZomboContentBlock::getBitmapFont(const char* fileName) const
	{
		return getMapValue(_bitmapFonts, fileName);
	}

	void ZomboContentBlock::update()
	{
		// todo: make update logic here
		if(_loadingQueue.empty())
		{
			_loaded = true;
			return;
		}
		std::string fileName = _loadingQueue.front();
		_loadingQueue.pop();

		loadResource(fileName);
	}

	void ZomboContentBlock::setRoot(const std::string &rootPath)
	{
		_rootPath = rootPath;
	}

	bool replace(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	std::string ZomboContentBlock::getBitmapFontFileName(const std::string& fileName)
	{
		std::string result;
		bool listen = false;
		for (uint i = 0; i < fileName.size(); i++)
		{
			if (fileName[i] == ']')
			{
				listen = false;
				continue;
			}
			if (fileName[i] == '[')
			{
				listen = true;
				continue;
			}
			if (!listen)
			{
				result += fileName[i];
			}
		}
		return result;
	}
	
	int ZomboContentBlock::getBitmapFontSize(const std::string& fileName)
	{
		std::string valueStr;
		std::string dimStr;
		bool listen = false;
		for (uint i = 0; i < fileName.size(); i++)
		{
			if (fileName[i] == ']') break;
			if (fileName[i] == '[')
			{
				listen = true;
				continue;
			}
			if (listen)
			{
				if (isdigit(fileName[i]))
				{
					valueStr += fileName[i];
				}
				else
				{
					dimStr += fileName[i];
				}
			}
		}
		if (valueStr.size() == 0) return 0;
		// todo: support em or other resolution independent dimensions
		return core::Convert::toInt(valueStr);
	}
	
	bool ZomboContentBlock::hasFormat(const std::string& fileName, const std::string& format)
	{
		return fileName.find(format) != fileName.npos;
	}

	std::string ZomboContentBlock::format(const std::string& fileName, int number, int leadingZeros)
	{
		std::string result = fileName;
		replace(result, "{n:000}", core::Convert::toString(number, leadingZeros));
		return result;
	}

	std::string ZomboContentBlock::format(const std::string& fileName, int number)
	{
		std::string result = fileName;
		replace(result, "{n}", core::Convert::toString(number));
		return result;
	}

	void ZomboContentBlock::loadResource(const std::string& fileName)
	{
		std::string ext = core::Path::getFileExt(fileName);
		if (ext == "png")
		{
			loadTexture(fileName);
		}
		else if (ext == "atlas")
		{
			// todo: test multipack
			if (hasFormat(fileName, "{n}"))
			{
				loadAtlas(fileName);
			}
			else
			{
				loadAtlasPage(fileName);
			}
		} if (ext == "otf"|| ext == "ttf")
		{
			int size = getBitmapFontSize(fileName);
			if (size > 0)
			{
				loadBitmapFont(fileName, size);
			}
			else
			{
				// todo: load outline vector font
			}
		}
	}

	void ZomboContentBlock::loadTexture(const std::string& fileName)
	{
	}

	ZomboContentAtlasPage* ZomboContentBlock::loadAtlasPage(const std::string& fileName)
	{
		resources::ContentManager content = resources::ContentManager::Default;
		std::string fullPath = _rootPath + fileName;
		if (!resources::ContentProvider::existContent(fullPath.c_str()))
		{
			// todo: handle error
			return nullptr;
		}
		resources::ContentProvider::openContent(fullPath.c_str());
		char * jsonString = static_cast<char *>(content.getBuffer());
		int len = 0;
		const int pageSize = 256 * 1024; 
		int count = 0;
		while ((count = resources::ContentProvider::read(jsonString + len, pageSize)) > 0)
		{
			len += count;
		}
		JsonAtlas atlas(jsonString, ZomboConstants::GameScale);			
		ZomboContentAtlasPage* page = atlas.createContentAtlasPage();
		resources::ContentProvider::closeContent();
		_altasPages[fileName] = page;

		std::string path = core::Path::getFilePath(fileName);
		graphics::Image2dData* imageData = content.loadUnregisteredTexture((_rootPath + path + page->texture.fileName).c_str());
		// todo: configure loading;
		page->texture.texture = new graphics::TextureImage2d(false, false);
		if (imageData != nullptr)
		{
			page->texture.texture->setData(imageData);
			page->texture.texture->create();
		}
		else
		{
			// todo: handle error
		}
		for (uint i = 0; i < page->images.size(); i++)
		{
			page->images[i]->texture = page->texture.texture;
			_images[fileName + "/" + page->images[i]->name] = page->images[i];
		}
		return page;
	}

	void ZomboContentBlock::loadAtlas(const std::string& fileName)
	{
		/// todo load pages while exists
		ZomboContentAtlas* altas = ZomboContentAtlas::create();
		altas->name = fileName;
		for (int i = 0; i < 100; i++) // todo: make constant for max atlas pages
		{
			std::string file = format(fileName.c_str(), i);
			if (!resources::ContentProvider::existContent(file.c_str()))
				break;
			ZomboContentAtlasPage* page = loadAtlasPage(file);
			altas->pages.push_back(page);
		}
		_altases[fileName] = altas;
	}

	void ZomboContentBlock::loadBitmapFont(const std::string& fileName, int size)
	{
		std::string fontFile = getBitmapFontFileName(fileName);
		resources::ContentManager content = resources::ContentManager::Default;
		std::string fullPath = _rootPath + fontFile;
		if (!resources::ContentProvider::existContent(fullPath.c_str()))
		{
			// todo: handle error
			ZomboLog::Default.e("file not found: " + fileName);
			return;
		}
		resources::ContentProvider::openContent(fullPath.c_str());
		char* buffer = static_cast<char *>(content.getBuffer());
		int len = 0;
		const int pageSize = 256 * 1024;
		int count = 0;
		while ((count = resources::ContentProvider::read(buffer + len, pageSize)) > 0)
		{
			len += count;
		}

		ZomboBitmapFont *font = ZomboBitmapFont::create();
		font->font = fonts::FontsManager::Default.createBitmapFont(buffer, len, static_cast<float>(size),
			&fonts::FontCharSet::all);
		_bitmapFonts[fileName] = font;
	}
}
