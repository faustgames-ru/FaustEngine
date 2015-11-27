#include "ZomboContentBlock.h"
#include "loaders/LoaderJsonAtlas.h"

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

	bool replace(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	bool ZomboContentBlock::hasFormat(const std::string& fileName)
	{
		return fileName.find("{n}") != fileName.npos;
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
			if (hasFormat(fileName))
			{
				loadAtlas(fileName);
			}
			else
			{
				loadAtlasPage(fileName);
			}
		}
	}

	void ZomboContentBlock::loadTexture(const std::string& fileName)
	{
	}

	void ZomboContentBlock::loadAtlasPage(const std::string& fileName)
	{
		resources::ContentManager content = resources::ContentManager::Default;
		if (!resources::ContentProvider::existContent(fileName.c_str()))
		{
			// todo: handle error
		}
		else
		{
			resources::ContentProvider::openContent(fileName.c_str());
			char * jsonString = (char *)content.getBuffer();
			int len = 0;
			const int pageSize = 256 * 1024; 
			int count = 0;
			while ((count = resources::ContentProvider::read(jsonString + len, pageSize)) > 0)
			{
				len += count;
			}
			JsonAtlas atlas(jsonString);
			for (uint i = 0; i < atlas.frames.size(); i++)
			{
				
			}
			resources::ContentProvider::closeContent();
		}
	}

	void ZomboContentBlock::loadAtlas(const std::string& fileName)
	{
		/// todo load pages while exists
	}
}
