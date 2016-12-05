#include "AtlasPacker.h"
#include "../../src_rectanglebinpack/MaxRectsBinPack.h"
#include "ContentManager.h"

namespace resources
{
	AtlasImageEntry::AtlasImageEntry(const char* p, const AtlasImageInput& e)
	{
		path = p;
		input = e;
	}

	AtlasRect::AtlasRect(rbp::Rect r)
	{
		rect = r; 
		entry = nullptr;
	}

	AlignInfo::AlignInfo(): blockSizeX(1), blockSizeY(1), borderBlockCount(1)
	{
	}

	int AlignInfo::alignWidth(int w) const
	{
		return borderBlockCount * 2 + core::Math::align(w, blockSizeX) / blockSizeX;
	}

	int AlignInfo::alignHeight(int h) const
	{
		return borderBlockCount * 2 + core::Math::align(h, blockSizeY) / blockSizeY;
	}

	int AlignInfo::alignPageWidth(int w) const
	{
		return core::Math::align(w, blockSizeX) / blockSizeX;
	}

	int AlignInfo::alignPageHeight(int h) const
	{
		return core::Math::align(h, blockSizeX) / blockSizeX;
	}

	AtlasPacker::AtlasPacker(llge::TextureImage2dFormat format, IAtlasPlacer* atlasPlacer)
	{
		_ready = false;
		_pageData = nullptr;
		pageSize = 0;

		alignInfo.blockSizeX = 1;
		alignInfo.blockSizeY = 1;
		alignInfo.borderBlockCount = 4;
		_internalFormat = format;
		placer = atlasPlacer;

		placer->SetupAlign(alignInfo);
	}

	AtlasPacker* AtlasPacker::create(llge::TextureImage2dFormat format)
	{
		IAtlasPlacer* placer = IAtlasPlacer::switchPlacer(format);
		if (placer == nullptr) 
			return nullptr;
		return new AtlasPacker(format, placer);
		/*
		switch (format)
		{
		case llge::Rgba: return new AtlasPacker(llge::Rgba);
		case llge::Rgb: return new AtlasPackerRgb(llge::Rgb);
		case llge::TextureFormatPvrtc12: break;
		case llge::TextureFormatPvrtc14: break;
		case llge::TextureFormatEtc1: break;
		case llge::TextureFormatAtc: break;
		case llge::TextureFormatEnumSize: break;
		default: break;
		}
		return nullptr;
		*/
	}

	void AtlasPacker::startPack(int size)
	{
		_ready = true;
		pageSize = core::Math::min(size, ContentManager::ImageMaxWidth);
		releaseEntries();
	}
	
	void AtlasPacker::add(const char* path, const AtlasImageInput& e)
	{
		_inputPack.push_back(new AtlasImageEntry(path, alignInput(e)));
	}

	void AtlasPacker::finishPack()
	{
		_ready = false;
	}

	bool compareRectSize(const rbp::RectSize &i, const rbp::RectSize &j)
	{
		int maxi = core::Math::max(i.width, i.height);
		int maxj = core::Math::max(j.width, j.height);
		if (maxi == maxj)
		{
			maxi = core::Math::min(i.width, i.height);
			maxj = core::Math::min(j.width, j.height);
		}
		return maxi < maxj;
	}

	void AtlasPacker::loadFiles()
	{
		releasePages();
		std::vector<rbp::RectSize> rects;
		for (uint i = 0; i < _inputPack.size(); i++)
		{
			rbp::RectSize size;
			size.width = _inputPack[i]->input.width;
			size.height = _inputPack[i]->input.height;
			rects.push_back(size);
		}

		sort(rects.begin(), rects.end(), compareRectSize);

		while (rects.size() > 0)
		{
			rbp::MaxRectsBinPack _pack(alignInfo.alignPageWidth(pageSize), alignInfo.alignPageHeight(pageSize));
			_pack.allowRotate = false;
			std::vector<rbp::Rect> packedRects;
			_pack.Insert(rects, packedRects, rbp::MaxRectsBinPack::RectBestShortSideFit);
			AtlasPage* page = new AtlasPage();
			_pages.push_back(page);
			for (uint i = 0; i < packedRects.size(); i++)
			{
				page->rects.push_back(AtlasRect(packedRects[i]));
			}
		}

		for (uint k = 0; k < _pages.size(); k++)
		{
			AtlasPage* page = _pages[k];
			for (uint j = 0; j < page->rects.size(); j++)
			{
				rbp::Rect rect = page->rects[j].rect;
				for (std::vector<AtlasImageEntry *>::iterator i = _inputPack.begin(); i != _inputPack.end(); ++i)
				{
					AtlasImageEntry* input = *i;
					float inputW = input->input.width;
					float inputH = input->input.height;
					if (inputW == rect.width && inputH == rect.height)
					{
						page->rects[j].entry = *i;
						_inputPack.erase(i);
						break;
					}
				}
			}
		}
		
		_pageData = new graphics::Image2dData(placer->getPageBufferSize(pageSize));
		_pageData->BlocksOrder = placer->getPageBlocksOrder();
		_pageData->Format = getFormat();
		_pageData->Width = pageSize;
		_pageData->Height = pageSize;
		_pageData->RawDataOffset = 0;

		PlaceArgs placeArgs;
		placeArgs.pageData = _pageData;
		// todo: cerate pages textures

		for (uint k = 0; k < _pages.size(); k++)
		{
			AtlasPage* page = _pages[k];
			graphics::TextureAtlasPage* texture = AtlasTexturesPool::Default.queryPage();
			for (uint i = 0; i < page->rects.size(); i++)
			{
				AtlasRect rect = page->rects[i];
				graphics::Image2dData* image = ContentManager::Default.loadUnregisteredTexture(page->rects[i].entry->path.c_str(), getQueryFormat());
				llge::TextureImage2dFormat format = graphics::Image2dFormat::ToLlgeFormat(image->Format);
				if (format != _internalFormat)
				{
					rect.entry->input.texture->AtlasEntry = false;
					rect.entry->input.texture->create();
					rect.entry->input.texture->setData(image);
					continue;
				}


				float inputW = alignInfo.alignWidth(image->Width);
				float inputH = alignInfo.alignHeight(image->Height);

				if (inputW == rect.rect.width && inputH == rect.rect.height)
				{
					if (placer == nullptr) continue;
					placeArgs.alignInfo = alignInfo;
					placeArgs.rect = rect.rect;
					placeArgs.imageData = image;

					placer->placeImage(placeArgs);

					rect.entry->input.texture->AtlasEntry = true;
					rect.entry->input.texture->setHandle(texture->getHandle());
					rect.entry->input.texture->transform = graphics::TextureTransform(
						static_cast<float>((rect.rect.x + alignInfo.borderBlockCount)*alignInfo.blockSizeX) / static_cast<float>(pageSize),
						static_cast<float>((rect.rect.y + alignInfo.borderBlockCount)*alignInfo.blockSizeY) / static_cast<float>(pageSize),
						static_cast<float>(image->Width / static_cast<float>(pageSize)),
						static_cast<float>(image->Height / static_cast<float>(pageSize)));
				}
				else if (inputH == rect.rect.width && inputW == rect.rect.height)
				{
					// todo: rotation support?
					inputH = inputH;
				}
				else 
				{
					inputH = inputH;
				}
			}
			texture->setData(_pageData);
		}
		delete _pageData;
		_pageData = nullptr;
	}

	bool AtlasPacker::ready()
	{
		return _ready;
	}

	AtlasImageInput AtlasPacker::alignInput(const AtlasImageInput& e)
	{
		AtlasImageInput result = e;
		result.width = alignInfo.alignWidth(result.width);
		result.height = alignInfo.alignWidth(result.height);
		return result;
	}

	llge::TextureQueryFormat AtlasPacker::getQueryFormat()
	{
		switch (_internalFormat)
		{
		case llge::TFRgba8888:
			return llge::TQFRgba8888;
		case llge::TFRgb888:
			return llge::TQFRgba8888;
		case llge::TFRgba4444:
			return llge::TQFRgba4444;
		default:
			return llge::TQFPlatformCompressed;
		}
	}
	graphics::Image2dFormat::e AtlasPacker::getFormat()
	{
		switch (_internalFormat)
		{
		case llge::TFRgba8888: 
			return graphics::Image2dFormat::Rgba;
		case llge::TFRgba4444:
			return graphics::Image2dFormat::Rgba4444;
		case llge::TFPvrtc14: 
			return graphics::Image2dFormat::Pvrtc14;
		case llge::TFAtc:
			return graphics::Image2dFormat::Atc;
		case llge::TFEtc2:
			return graphics::Image2dFormat::Etc2;
		default:
			return graphics::Image2dFormat::Rgba;
		}
	}

	void AtlasPacker::releasePages()
	{
		for (uint i = 0; i < _pages.size(); i++)
		{
			delete _pages[i];
		}
		_pages.clear();
	}

	void AtlasPacker::releaseEntries()
	{
		for (uint i = 0; i < _inputPack.size(); i++)
		{
			delete _inputPack[i];
		}
		_inputPack.clear();
	}

	IAtlasPlacer* IAtlasPlacer::rgba8888 = new AtlasPlacerRgba();
	IAtlasPlacer* IAtlasPlacer::rgba4444 = new AtlasPlacerRgba();
	IAtlasPlacer* IAtlasPlacer::pvrtc14 = new AtlasPlacerPvrtc14();
	IAtlasPlacer* IAtlasPlacer::etc2 = new AtlasPlacerEtc2();
	IAtlasPlacer* IAtlasPlacer::atc = new AtlasPlacerAtc();

	IAtlasPlacer* IAtlasPlacer::switchPlacer(llge::TextureImage2dFormat format)
	{
		switch (format)
		{
		case llge::TFRgba8888:
			return rgba8888;
		case llge::TFRgba4444:
			return rgba4444;
		case llge::TFPvrtc14:
			return pvrtc14;
		case llge::TFAtc:
			return atc;
		case llge::TFEtc2:
			return etc2;
		default:
			return nullptr;
		}
	}

	graphics::Image2dBlocksOrder::e IAtlasPlacer::getPageBlocksOrder()
	{
		return graphics::Image2dBlocksOrder::Normal;
	}

	AtlasTexturesPool AtlasTexturesPool::Default;
		
	graphics::TextureAtlasPage* AtlasTexturesPool::queryPage()
	{
		_index++;
		graphics::TextureAtlasPage *result;
		if (_index == _pages.size())
		{
			result = new graphics::TextureAtlasPage(true);
			result->create();
			_pages.push_back(result);
			return result;
		}
		result = _pages[_index];
		return result;
	}

	void AtlasTexturesPool::clear()
	{
		_index = -1;
	}

	int AtlasPlacerRgba::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize;
	}

	void AtlasPlacerRgba::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImage<int>(e);
	}

	int AtlasPlacerRgba4444::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 2;
	}

	void AtlasPlacerRgba4444::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImage<short>(e);
	}

	int AtlasPlacerRgb::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize*3/4;
	}

	void AtlasPlacerRgb::placeImage(const PlaceArgs& e)
	{
	}

	int AtlasPlacerPvrtc14::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 8;
	}

	void AtlasPlacerPvrtc14::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImageWithBorder<int64_t>(e);
	}

	void AtlasPlacerPvrtc14::SetupAlign(AlignInfo& alignInfo)
	{
		alignInfo.blockSizeX = 4;
		alignInfo.blockSizeY = 4;
		alignInfo.borderBlockCount = 1;
	}

	int AtlasPlacerAtc::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 4;
	}

	struct int128
	{
		int64_t h;
		int64_t l;
	};

	void AtlasPlacerAtc::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImageWithBorder<int128>(e);
	}

	void AtlasPlacerAtc::SetupAlign(AlignInfo& alignInfo)
	{
		alignInfo.blockSizeX = 4;
		alignInfo.blockSizeY = 4;
		alignInfo.borderBlockCount = 1;
	}

	int AtlasPlacerEtc2::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 4;
	}

	void AtlasPlacerEtc2::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImageWithBorder<int128>(e);
	}

	void AtlasPlacerEtc2::SetupAlign(AlignInfo& alignInfo)
	{
		alignInfo.blockSizeX = 4;
		alignInfo.blockSizeY = 4;
		alignInfo.borderBlockCount = 1;
	}
}