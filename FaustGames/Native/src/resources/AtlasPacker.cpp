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
					if (inputW == rect.width && inputH == rect.height ||
						inputW == rect.height && inputH == rect.width)
					{
						page->rects[j].entry = *i;
						_inputPack.erase(i);
						break;
					}
				}
			}
		}
		
		_pageData = new graphics::Image2dData(placer->getPageBufferSize(pageSize));
		_pageData->Format = getFormat();
		_pageData->Width = pageSize;
		_pageData->Height = pageSize;

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
				float inputW = alignInfo.alignWidth(image->Width);
				float inputH = alignInfo.alignHeight(image->Height);

				if (inputW == rect.rect.width && inputH == rect.rect.height)
				{
					if (placer == nullptr) continue;
					placeArgs.alignInfo = alignInfo;
					placeArgs.rect = rect.rect;
					placeArgs.imageData = image;
					placer->placeImage(placeArgs);

					rect.entry->input.texture->setHandle(texture->getHandle());
					rect.entry->input.texture->transform = graphics::TextureTransform(
						static_cast<float>((rect.rect.x + alignInfo.borderBlockCount)*alignInfo.blockSizeX) / static_cast<float>(pageSize),
						static_cast<float>((rect.rect.y + alignInfo.borderBlockCount)*alignInfo.blockSizeY) / static_cast<float>(pageSize),
						static_cast<float>(image->Width - alignInfo.borderBlockCount*alignInfo.blockSizeX * 2) / static_cast<float>(pageSize),
						static_cast<float>(image->Height - alignInfo.borderBlockCount*alignInfo.blockSizeY * 2) / static_cast<float>(pageSize));

					/*
					rect.entry->input.texture->transform = graphics::TextureTransform(
						scaleX(rect.rect.x + pixelBorders),
						scaleX(rect.rect.y + pixelBorders),
						scaleX(inputW - pixelBorders * 2),
						scaleX(inputH - pixelBorders * 2));
					*/
					
					/*
					rect.rect.x *= sizeAlign;
					rect.rect.y *= sizeAlign;
					rect.rect.width *= sizeAlign;
					rect.rect.width *= sizeAlign;
					rect.entry->input.texture->setHandle(texture->getHandle());
					rect.entry->input.texture->transform = graphics::TextureTransform(
						scaleX(rect.rect.x + pixelBorders),
						scaleX(rect.rect.y + pixelBorders),
						scaleX(inputW - pixelBorders * 2),
						scaleX(inputH - pixelBorders * 2));
					placeImage(rect, image);
					*/
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
		case llge::TFRgb888: 
			return graphics::Image2dFormat::Rgb;
		case llge::TFRgba4444:
			return graphics::Image2dFormat::Rgba4444;
		case llge::TFPvrtc12:
			return graphics::Image2dFormat::Pvrtc12;
		case llge::TFPvrtc14: 
			return graphics::Image2dFormat::Pvrtc14;
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
	IAtlasPlacer* IAtlasPlacer::rgb888 = new AtlasPlacerRgb();
	IAtlasPlacer* IAtlasPlacer::pvrtc12 = new AtlasPlacerPvrtc12();
	IAtlasPlacer* IAtlasPlacer::pvrtc14 = new AtlasPlacerPvrtc14();

	IAtlasPlacer* IAtlasPlacer::switchPlacer(llge::TextureImage2dFormat format)
	{
		switch (format)
		{
		case llge::TFRgba8888:
			return rgba8888;
		case llge::TFRgba4444:
			return rgba4444;
		case llge::TFRgb888:
			return rgb888;
		case llge::TFPvrtc12:
			return pvrtc14;
		case llge::TFPvrtc14:
			return pvrtc12;
		default:
			return nullptr;
		}
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

	int AtlasPlacerPvrtc12::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 16;
	}

	void AtlasPlacerPvrtc12::placeImage(const PlaceArgs& e)
	{
	}

	int AtlasPlacerPvrtc14::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 8;
	}

	void AtlasPlacerPvrtc14::placeImage(const PlaceArgs& e)
	{
	}

	/*

	AtlasPackerRgb::AtlasPackerRgb(llge::TextureImage2dFormat format) : AtlasPacker(format)
	{
	}

	void AtlasPackerRgb::createPageData()
	{
		_pageData = new graphics::Image2dData(pageSize * pageSize * 3 / 4);
		_pageData->Format = graphics::Image2dFormat::Rgb;
	}

	void AtlasPackerRgb::clearPageData()
	{
	}

	void AtlasPackerRgb::placeImage(AtlasRect rect, graphics::Image2dData* image)
	{
		switch (image->Format)
		{
		case graphics::Image2dFormat::Rgb:
			placeRgb(rect, image);
			break;
		case graphics::Image2dFormat::Rgba:
		case graphics::Image2dFormat::Pvrtc12:
		case graphics::Image2dFormat::Pvrtc14:
		case graphics::Image2dFormat::Etc1:
			break;
		default: break;
		}
	}


	void AtlasPackerRgb::placeRgbRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData *image)
	{
		
		int rowDstBase = (rect.rect.y + pixelBorders) * _pageSize + rect.rect.x + pixelBorders;
		int rowSrcBase = 0;
		byte* imagePixels = reinterpret_cast<byte*>(image->Pixels);
		byte* pagePixels = reinterpret_cast<byte*>(_pageData->Pixels);
		int channels = 3;
		int step = image->Width * channels;
		int newStep = (step / 4) * 4;
		if (newStep < step)
		{
			step = newStep + 4;
		}

		int w = rect.rect.width - pixelBorders * 2;

		int rowDst = rowDstBase + _pageSize * yDst;
		int rowSrc = rowSrcBase + step * ySrc;
		int bytesCount = w*channels;
		for (int x = 0; x < bytesCount; x++)
		{
			*(imagePixels + rowSrc) = *(pagePixels + rowSrc);
			rowDst ++;
			rowSrc ++;
		}
		
	}

	void AtlasPackerRgb::placeRgbCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData *image)
	{
		
		int rowDstBase = (rect.rect.y + pixelBorders) * _pageSize + rect.rect.x + pixelBorders;
		int rowSrcBase = 0;
		byte* imagePixels = reinterpret_cast<byte*>(image->Pixels);
		byte* pagePixels = reinterpret_cast<byte*>(_pageData->Pixels);
		int channels = 3;
		int step = image->Width * channels;
		int newStep = (step / 4) * 4;
		if (newStep < step)
		{
			step = newStep + 4;
		}

		int w = rect.rect.width - pixelBorders * 2;
		int h = rect.rect.height - pixelBorders * 2;

		int rowDst = rowDstBase + xDst;
		int rowSrc = rowSrcBase + xSrc;
		for (int y = 0; y < h; y++)
		{
			*(imagePixels + rowSrc + 0) = *(pagePixels + rowSrc + 0);
			*(imagePixels + rowSrc + 1) = *(pagePixels + rowSrc + 1);
			*(imagePixels + rowSrc + 2) = *(pagePixels + rowSrc + 2);
			rowDst += w*channels;
			rowSrc += channels*step;
		}
		
	}

	void AtlasPackerRgb::placeRgb(AtlasRect rect, graphics::Image2dData* image)
	{
		
		int h = rect.rect.height - pixelBorders * 2;
		int w = rect.rect.width - pixelBorders * 2;
		for (int y = 0; y < h; y++)
		{
			placeRgbRow(rect, y, y, image);
		}

		for (int y = 1; y <= pixelBorders; y++)
		{
			placeRgbRow(rect, 0, 0 - y, image);
			placeRgbRow(rect, h - 1, h - 1 + y, image);
		}
		for (int x = 1; x <= pixelBorders; x++)
		{
			placeRgbCol(rect, 0, 0 - x, image);
			placeRgbCol(rect, w - 1, w - 1 + x, image);
		}
		
	}

	AtlasPackerRgba::AtlasPackerRgba(llge::TextureImage2dFormat format) : AtlasPacker(format)
	{
	}

	void AtlasPackerRgba::createPageData()
	{
		_pageData = new graphics::Image2dData(pageSize, pageSize);
		_pageData->Format = graphics::Image2dFormat::Rgba;
	}

	void AtlasPackerRgba::clearPageData()
	{
		int size = _pageData->Width*_pageData->Height;
		for (int i = 0; i < size; i++)
		{
			_pageData->Pixels[i] = 0;
		}
	}

	void AtlasPackerRgba::placeImage(AtlasRect rect, graphics::Image2dData* image)
	{
		switch (image->Format)
		{
		case graphics::Image2dFormat::Rgba:
			placeRgba(rect, image);
			break;
		case graphics::Image2dFormat::Rgb:
			placeRgb(rect, image);
			break;
		case graphics::Image2dFormat::Pvrtc14:
		case graphics::Image2dFormat::Pvrtc12:
		case graphics::Image2dFormat::Etc1:
			break;
		default: 
			break;
		}
	}
	
	void AtlasPackerRgba::placeRgbRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData *image)
	{
		
		int rowDstBase = (rect.rect.y + pixelBorders) * _pageSize + rect.rect.x + pixelBorders;
		int rowSrcBase = 0;
		byte* imagePixels = reinterpret_cast<byte*>(image->Pixels);
		int channels = 3;
		int step = image->Width * channels;
		int newStep = (step / 4) * 4;
		if (newStep < step)
		{
			step = newStep + 4;
		}

		int w = rect.rect.width - pixelBorders * 2;

		int rowDst = rowDstBase + _pageSize * yDst;
		int rowSrc = rowSrcBase + step * ySrc;
		for (int x = 0; x < w; x++)
		{

			uint c = *reinterpret_cast<uint *>(imagePixels + rowSrc);
			_pageData->Pixels[rowDst] = (c >> 8) | 0xff000000;
			rowDst++;
			rowSrc += channels;
		}
		
	}

	void AtlasPackerRgba::placeRgbCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData *image)
	{
		
		int rowDstBase = (rect.rect.y + pixelBorders) * _pageSize + rect.rect.x + pixelBorders;
		int rowSrcBase = 0;
		byte* imagePixels = reinterpret_cast<byte*>(image->Pixels);
		int channels = 3;
		int step = image->Width * channels;
		int newStep = (step / 4) * 4;
		if (newStep < step)
		{
			step = newStep + 4;
		}

		int w = rect.rect.width - pixelBorders * 2;
		int h = rect.rect.height - pixelBorders * 2;

		int rowDst = rowDstBase + xDst;
		int rowSrc = rowSrcBase + xSrc;
		for (int y = 0; y < h; y++)
		{

			uint c = *reinterpret_cast<uint *>(imagePixels + rowSrc);
			_pageData->Pixels[rowDst] = (c >> 8) | 0xff000000;
			rowDst += w;
			rowSrc += channels*step;
		}
		
	}

	void AtlasPackerRgba::placeRgbaRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* image)
	{
		
		int rowDstBase = (rect.rect.y + pixelBorders) * _pageSize + (rect.rect.x + pixelBorders);
		int rowSrcBase = 0;
		int w = rect.rect.width - pixelBorders * 2;

		int rowDst = rowDstBase + _pageSize*yDst;
		int rowSrc = rowSrcBase + image->Width*ySrc;
		for (int x = 0; x < w; x++)
		{
			_pageData->Pixels[rowDst] = image->Pixels[rowSrc];
			rowDst++;
			rowSrc++;
		}
		
	}

	void AtlasPackerRgba::placeRgbaCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData *image)
	{
		
		int rowDstBase = (rect.rect.y + pixelBorders) * _pageSize + (rect.rect.x + pixelBorders);
		int rowSrcBase = 0;
		int h = rect.rect.height - pixelBorders * 2;

		int rowDst = rowDstBase + xDst;
		int rowSrc = rowSrcBase + xSrc;
		for (int y = 0; y < h; y++)
		{
			_pageData->Pixels[rowDst] = image->Pixels[rowSrc];
			rowDst += _pageSize;
			rowSrc += image->Width;
		}
		
	}


	void AtlasPackerRgba::placeRgb(AtlasRect rect, graphics::Image2dData* image)
	{
		
		int h = rect.rect.height - pixelBorders * 2;
		int w = rect.rect.width - pixelBorders * 2;
		for (int y = 0; y < h; y++)
		{
			placeRgbRow(rect, y, y, image);
		}

		for (int y = 1; y <= pixelBorders; y++)
		{
			placeRgbRow(rect, 0, 0 - y, image);
			placeRgbRow(rect, h - 1, h - 1 + y, image);
		}
		for (int x = 1; x <= pixelBorders; x++)
		{
			placeRgbCol(rect, 0, 0 - x, image);
			placeRgbCol(rect, w - 1, w - 1 + x, image);
		}
		
	}

	void AtlasPackerRgba::placeRgba(AtlasRect rect, graphics::Image2dData* image)
	{
		
		int h = rect.rect.height - pixelBorders * 2;
		int w = rect.rect.width - pixelBorders * 2;
		for (int y = 0; y < h; y++)
		{
			placeRgbaRow(rect, y, y, image);
		}

		for (int y = 1; y <= pixelBorders; y++)
		{
			placeRgbaRow(rect, 0, 0 - y, image);
			placeRgbaRow(rect, h - 1, h - 1 + y, image);
		}
		for (int x = 1; x <= pixelBorders; x++)
		{
			placeRgbaCol(rect, 0, 0 - x, image);
			placeRgbaCol(rect, w - 1, w - 1 + x, image);
		}
	
	}


	AtlasPackerPvr14::AtlasPackerPvr14(llge::TextureImage2dFormat format) : AtlasPacker(format)
	{
	}

	void AtlasPackerPvr14::createPageData()
	{
		_pageData = new graphics::Image2dData(pageSize * pageSize / 8);
		_pageData->Format = graphics::Image2dFormat::Pvrtc12;
	}

	void AtlasPackerPvr14::clearPageData()
	{
	}

	void AtlasPackerPvr14::placeImage(AtlasRect rect, graphics::Image2dData* image)
	{
		switch (image->Format)
		{
		case graphics::Image2dFormat::Pvrtc14:
			place(rect, image);
			break;
		case graphics::Image2dFormat::Rgb:
		case graphics::Image2dFormat::Rgba:
		case graphics::Image2dFormat::Pvrtc12:
		case graphics::Image2dFormat::Etc1:
			break;
		default: break;
		}
	}

	void AtlasPackerPvr14::place(AtlasRect rect, graphics::Image2dData* imageData)
	{
	}

	void AtlasPackerPvr14::placeRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData)
	{
	}

	AtlasTexturesPool AtlasTexturesPool::Default;
	*/
}