#include "TexturesManager.h"

namespace resources
{
	TextureEntry::TextureEntry(const std::string& n, graphics::TextureImage2d* i, llge::TextureQueryFormat qf)
	{
		name = n;
		image = i;
		queryFormat = qf;		
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
		_atlasBuildStarted = true;
	}

	void TexturesManager::finishAtlasBuild()
	{
		_atlasBuildStarted = false;
		executeAtlasLoadingList();
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
		_atlasLoadingList.clear();
		executeUnloadingList();
		executeDisposeList();
		clearAtlasPages();
	}

	void TexturesManager::fillStatistics(IntPtr statistics)
	{
		llge::TexturesManagerStatistics* stat = static_cast<llge::TexturesManagerStatistics*>(statistics);
		stat->atalsPagesCount = _atlasPages.size();
		stat->imagesCount = _disposeList.size();
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
		auto entry = new TextureEntry(name_str, image, queryFormat);
		_disposeList.push_back(entry);
		_entries[name_str] = entry;
		if (_atlasBuildStarted)
		{
			TextureAtlasEntry atlasEntry;
			atlasEntry.imageInfo = _texturesLoader->loadTextureSize(name, queryFormat);
			atlasEntry.entry = entry;
			_atlasLoadingList.push_back(atlasEntry);
		}
		else
		{
			_loadingList.push_back(entry);
		}
		return image;
	}

	void TexturesManager::executeAtlasLoadingList()
	{
		// todo build atals pages
		for (int i = 0; i < llge::TFEnumSize; i++)
		{
			auto packer = TexturesAtlasPacker::create(static_cast<llge::TextureImage2dFormat>(i));
			if (packer == nullptr) continue;
			std::vector<graphics::TextureAtlasPage*> pages;
			packer->pack(_texturesLoader, _atlasLoadingList, pages);
			for (int j = 0; j < pages.size(); j++)
			{
				_atlasPages.push_back(pages[j]);
			}
			delete packer;
		}
		_atlasLoadingList.clear();
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

	void TexturesManager::clearAtlasPages()
	{
		for (int i = 0; i < _atlasPages.size(); i++)
		{
			unloadPage(_atlasPages[i]);
			disposePage(_atlasPages[i]);
		}
		_atlasPages.clear();
	}

	void TexturesManager::loadEntry(TextureEntry* entry) const
	{
		entry->image->create();
		entry->loadImage(_texturesLoader);
	}

	void TexturesManager::unloadEntry(TextureEntry* entry)
	{
		entry->image->cleanup();
	}

	void TexturesManager::unloadPage(graphics::TextureAtlasPage* page)
	{
		page->cleanup();
	}

	void TexturesManager::disposePage(graphics::TextureAtlasPage* page)
	{
		graphics::TexturesPool::ReturnAtlasPage(page);
	}

	void TexturesManager::disposeEntry(TextureEntry* entry)
	{
		graphics::TexturesPool::ReturnImage(entry->image);
		delete entry;
	}

	TexturesAtlasPacker* TexturesAtlasPacker::create(llge::TextureImage2dFormat format)
	{
		IAtlasPlacer* placer = IAtlasPlacer::switchPlacer(format);
		if (placer == nullptr)
			return nullptr;
		return new TexturesAtlasPacker(format, placer);
	}

	TexturesAtlasPacker::TexturesAtlasPacker(llge::TextureImage2dFormat format, IAtlasPlacer* atlasPlacer)
	{
		alignInfo.blockSizeX = 1;
		alignInfo.blockSizeY = 1;
		alignInfo.borderBlockCount = 4;
		_internalFormat = format;
		placer = atlasPlacer;
		placer->SetupAlign(alignInfo);
	}

	struct TexturesAtlasRectSize
	{
		TextureAtlasEntry entry;
		int width;
		int height;
	};


	bool compareRectSize(const TexturesAtlasRectSize &i, const TexturesAtlasRectSize &j)
	{		
		int maxi = core::Math::max(i.width, i.height);
		int maxj = core::Math::max(j.width, j.height);
		if (maxi == maxj)
		{
			maxi = core::Math::min(i.width, i.height);
			maxj = core::Math::min(j.width, j.height);
		}
		return maxi > maxj;		
	}

	class TexturesAtlasRect
	{
	public:
		BinPackRect rect;
		TextureAtlasEntry entry;
		TexturesAtlasRect(BinPackRect r, TextureAtlasEntry e)
		{
			rect = r;
			entry = e;
		}
	};

	struct Size2i
	{
		int w;
		int h;
		void MakePot()
		{
			w = core::Math::pot(w);
			h = core::Math::pot(h);
		}
		void MakeSqurePot()
		{
			w = core::Math::pot(w);
			h = core::Math::pot(h);
			if (w < h)
			{
				w = h;
			}
			else
			{
				h = w;
			}
		}
	};

	class TexturesAtlasPage
	{
	public:
		std::vector<TexturesAtlasRect> rects;
		BinPack Pack;
		Size2i calcSize();
	};

	Size2i TexturesAtlasPage::calcSize()
	{
		Size2i size; 
		size.w = 0;
		size.h = 0;
		for (int i = 0; i < rects.size(); i++)
		{
			auto r = rects[i].rect;
			int x = r.x + r.width;
			int y = r.y + r.height;
			if (size.w < x)
			{
				size.w = x;
			}
			if (size.h < y)
			{
				size.h = y;
			}
		}
		return size;
	}

	void TexturesAtlasPacker::pack(TexturesLoader* texturesLoader, const std::vector<TextureAtlasEntry>& e, std::vector<graphics::TextureAtlasPage*>& resultPages)
	{
		std::vector<TextureAtlasEntry> entries;
		auto atlasFormat = graphics::Image2dFormat::FromLlgeFormat(_internalFormat);

		int pageSize = graphics::GraphicsDevice::getMaxTextureSize();
		if (pageSize > 4096)
		{
			pageSize = 4096;
		}
		if (atlasFormat == graphics::Image2dFormat::Rgb || 
			atlasFormat == graphics::Image2dFormat::Rgba || 
			atlasFormat == graphics::Image2dFormat::Rgba4444)
		{
			pageSize = 2048;
		}
		alignInfo.pageSize = pageSize;


		for (int i = 0; i < e.size(); i++)
		{
			if (e[i].imageInfo.Format != atlasFormat) continue;
			entries.push_back(e[i]);
		}
		if (entries.size() == 0) return;

		std::vector<TexturesAtlasRectSize> rects;

		for (uint i = 0; i < entries.size(); i++)
		{
			TexturesAtlasRectSize size;

			auto entry = entries[i];
			if (entry.imageInfo.Width == 10)
			{
				entry.imageInfo.Width = 10;
			}
			size.width = alignInfo.alignWidth(entry.imageInfo.Width);
			size.height = alignInfo.alignHeight(entry.imageInfo.Height);
			size.entry = entry;
			rects.push_back(size);
		}

		sort(rects.begin(), rects.end(), compareRectSize);

		std::vector<TexturesAtlasPage* > pages;
		for (uint i = 0; i < rects.size(); i++)
		{
			BinPackNode* res = nullptr;
			TexturesAtlasPage* page = nullptr;
			for (uint j = 0; j < pages.size(); j++)
			{
				res = pages[j]->Pack.insert(rects[i].width, rects[i].height);
				if (res != nullptr)
				{
					page = pages[j];
					break;
				}
			}
			if (page == nullptr)
			{
				TexturesAtlasPage* newPage = new TexturesAtlasPage();
				newPage->Pack.clear(alignInfo.alignPageWidth(pageSize), alignInfo.alignPageHeight(pageSize));
				pages.push_back(newPage);
				res = newPage->Pack.insert(rects[i].width, rects[i].height);
				if (res != nullptr)
				{
					auto r = TexturesAtlasRect(res->inserted, rects[i].entry);
					
					newPage->rects.push_back(r);
				}
				else
				{
					// log error
				}
			}
			else
			{
				auto r = TexturesAtlasRect(res->inserted, rects[i].entry);
				page->rects.push_back(r);
			}
		}

		int sizeBufferSize = placer->getPageBufferSize(pageSize);
		auto pageData = new graphics::Image2dData(sizeBufferSize);
		pageData->BlocksOrder = placer->getPageBlocksOrder();
		pageData->Format = atlasFormat;
		pageData->Width = pageSize;
		pageData->Height = pageSize;
		pageData->RawDataOffset = 0;
		memset(pageData->Pixels, 0, sizeBufferSize * 4);

		PlaceArgs placeArgs;
		placeArgs.pageData = pageData;

		// todo: cerate pages textures
		for (uint k = 0; k < pages.size(); k++)
		{
			TexturesAtlasPage* page = pages[k];
			auto currentPageSize = page->calcSize();
			if (atlasFormat == graphics::Image2dFormat::Pvrtc12 || atlasFormat == graphics::Image2dFormat::Pvrtc14)
			{
				currentPageSize.MakeSqurePot();
			}
			else  if (atlasFormat == graphics::Image2dFormat::Rgb)
			{
				currentPageSize.MakePot();
			}
			pageData->Width = currentPageSize.w*alignInfo.blockSizeX;
			pageData->Height = currentPageSize.h*alignInfo.blockSizeY;

			graphics::TextureAtlasPage* texture = graphics::TexturesPool::GetAtlasPage();
			texture->create();
			resultPages.push_back(texture);
			if (pageData->Format == graphics::Image2dFormat::Etc1)
			{
				texture->createAlphaIfNeeded();
			}
			for (uint i = 0; i < page->rects.size(); i++)
			{

				TexturesAtlasRect rect = page->rects[i];
				auto textureTransform = alignInfo.createTextureTransform(
					rect.rect,
					rect.entry.imageInfo.Width,
					rect.entry.imageInfo.Height,
					pageData->Width,
					pageData->Height);

				auto image = texturesLoader->readImage(rect.entry.entry->name.c_str(), rect.entry.entry->queryFormat);

				if (image.Format != atlasFormat)
				{
					// warn error!!
					continue;
				}

				int inputW = alignInfo.alignWidth(image.Width);
				int inputH = alignInfo.alignHeight(image.Height);

				if (inputW == rect.rect.width && inputH == rect.rect.height)
				{
					if (placer == nullptr) continue;

					graphics::Image2dData imageData(image);


					placeArgs.alignInfo = alignInfo;
					placeArgs.rect = rect.rect;
					placeArgs.imageData = &imageData;

					placer->placeImage(placeArgs);

					rect.entry.entry->image->associate(texture);
					rect.entry.entry->image->transform = textureTransform;

					imageData.Pixels = nullptr; //disallow delete pixels buffer on dispose
				}
				else
				{
					// warn error!!
					continue;
				}
			}
			texture->setData(pageData);
			memset(pageData->Pixels, 0, sizeBufferSize * 4);
		}

		delete pageData;
		alignInfo.pageSize = -1;
	}
}
