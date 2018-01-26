#include "TexturesManager.h"

namespace resources
{
	TextureEntry::TextureEntry(const std::string& n, graphics::TextureImage2d* i, llge::TextureQueryFormat qf)
	{
		name = n;
		image = i;
		queryFormat = qf;
		imageInfo = nullptr;
	}

	void TextureEntry::loadImage(TexturesLoader* loader) const
	{
		auto imageData = loader->readImage(name.c_str(), queryFormat);
		image->setData(imageData);
	}

	TexturesManager::TexturesManager(TexturesLoader* texturesLoader)
	{
		_texturesLoader = texturesLoader;
		_atlasBuildStarted = false;
	}

	TexturesManager::~TexturesManager()
	{
	}

	void TexturesManager::startAtlasBuild()
	{

		//_atlasBuildStarted = true;
	}

	void TexturesManager::finishAtlasBuild()
	{
		// todo: build atlas
		// todo: load images to pages
		
		//_atlasBuildStarted = false;
	}

	llge::ITextureImage2d* TexturesManager::loadImage(const char* name, llge::TextureQueryFormat queryFormat)
	{
		return loadTextureImage(name, queryFormat);
	}

	void TexturesManager::update()
	{
		executeLoadingList();
	}

	void TexturesManager::clear()
	{
		_entries.clear();
		_loadingList.clear();
		executeUnloadingList();
		// todo: unload atlas pages
		executeDisposeList();
	}

	void TexturesManager::dispose()
	{
		delete this;
	}

	graphics::TextureImage2d* TexturesManager::loadTextureImage(const char* name, llge::TextureQueryFormat queryFormat)
	{
		std::string name_str = name;
		auto existing = _entries.find(name_str);

		if (existing != _entries.end())
		{
			return existing->second->image;
		}

		graphics::TextureImage2d* image = graphics::TexturesPool::GetImage();
		TextureEntry* entry = new TextureEntry(name_str, image, queryFormat);
		_disposeList.push_back(entry);
		_entries[name_str] = entry;
		if (_atlasBuildStarted)
		{
			// todo: load image size
			// todo: add image to atlas
		}
		else
		{
			_loadingList.push_back(entry);
		}
		return image;
	}

	void TexturesManager::executeLoadingList()
	{
		for (int i = 0; i < _loadingList.size(); i++)
		{
			loadEntry(_loadingList[i]);
			_unloadingList.push_back(_loadingList[i]);
		}
		_loadingList.clear();
	}

	void TexturesManager::executeUnloadingList()
	{
		for (int i = 0; i < _unloadingList.size(); i++)
		{
			unloadEntry(_unloadingList[i]);
		}
		_unloadingList.clear();
	}

	void TexturesManager::executeDisposeList()
	{
		for (int i = 0; i < _disposeList.size(); i++)
		{
			disposeEntry(_disposeList[i]);
		}
		_disposeList.clear();
	}

	void TexturesManager::loadEntry(TextureEntry* entry)
	{
		entry->image->create();
		entry->loadImage(_texturesLoader);
	}

	void TexturesManager::unloadEntry(TextureEntry* entry)
	{
		entry->image->cleanup();
	}

	void TexturesManager::disposeEntry(TextureEntry* entry)
	{
		graphics::TexturesPool::ReturnImage(entry->image);
		delete entry;
	}

}
