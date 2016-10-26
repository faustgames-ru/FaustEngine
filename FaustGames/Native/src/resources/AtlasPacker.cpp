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
		
	AtlasPacker::AtlasPacker(llge::TextureImage2dFormat format)
	{
		_ready = false;
		_pageData = nullptr;
		_pageSize = 0;
		sizeAlign = 1;
		sizeBorders = 1;
		pixelBorders = sizeAlign * sizeBorders;
		_internalFormat = format;
	}

	AtlasPacker* AtlasPacker::create(llge::TextureImage2dFormat format)
	{
		switch (format)
		{
		case llge::Rgba: return new AtlasPackerRgba(llge::Rgba);
		case llge::Rgb: return new AtlasPackerRgb(llge::Rgb);
		case llge::TextureFormatPvrtc12: break;
		case llge::TextureFormatPvrtc14: break;
		case llge::TextureFormatEtc1: break;
		case llge::TextureFormatAtc: break;
		case llge::TextureFormatEnumSize: break;
		default: break;
		}
		return nullptr;
	}

	void AtlasPacker::startPack(int pageSize)
	{
		_ready = true;
		_pageSize = core::Math::min(pageSize, ContentManager::ImageMaxWidth);
		releaseEntries();
	}
	
	void AtlasPacker::add(const char* path, const AtlasImageInput& e)
	{
		_inputPack.push_back(new AtlasImageEntry(path, e));
	}

	void AtlasPacker::finishPack()
	{
		_ready = false;
	}

	bool compareRectSize(const rbp::RectSize &i, const rbp::RectSize &j)
	{
		return core::Math::max(i.width, i.height)>core::Math::max(j.width, j.height);
	}

	void AtlasPacker::loadFiles()
	{
		releasePages();
		std::vector<rbp::RectSize> rects;
		for (uint i = 0; i < _inputPack.size(); i++)
		{
			rbp::RectSize size = rbp::RectSize();
			size.width = getAlignedSizeWithBorder(_inputPack[i]->input.width);
			size.height = getAlignedSizeWithBorder(_inputPack[i]->input.height);
			rects.push_back(size);
		}

		sort(rects.begin(), rects.end(), compareRectSize);

		while (rects.size() > 0)
		{
			rbp::MaxRectsBinPack _pack(getAlignedSize(_pageSize), getAlignedSize(_pageSize));
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
					float inputW = getAlignedSizeWithBorder(input->input.width);
					float inputH = getAlignedSizeWithBorder(input->input.height);
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
		
		createPageData();
		// todo: cerate pages textures
		for (uint k = 0; k < _pages.size(); k++)
		{
			AtlasPage* page = _pages[k];
			graphics::TextureAtlasPage* texture = AtlasTexturesPool::Default.queryPage();
			for (uint i = 0; i < page->rects.size(); i++)
			{
				AtlasRect rect = page->rects[i];
				graphics::Image2dData* image = ContentManager::Default.loadUnregisteredTexture(page->rects[i].entry->path.c_str());
				float inputW = getAlignedSizeWithBorder(image->Width);
				float inputH = getAlignedSizeWithBorder(image->Height);

				if (inputW == rect.rect.width && inputH == rect.rect.height)
				{
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
				}
				else if (inputH == rect.rect.width && inputW == rect.rect.height)
				{
					// todo: rotation support?
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

	float AtlasPacker::getAlignedSizeWithBorder(float size)
	{
		return getAlignedSize(size) + sizeBorders * 2;
	}

	float AtlasPacker::getAlignedSize(float size)
	{
		return size / sizeAlign;
	}

	float AtlasPacker::scaleX(int x)
	{
		return static_cast<float>(x) / static_cast<float>(_pageSize);
	}

	float AtlasPacker::scaleY(int y)
	{
		return static_cast<float>(y) / static_cast<float>(_pageSize);
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


	AtlasPackerRgb::AtlasPackerRgb(llge::TextureImage2dFormat format) : AtlasPacker(format)
	{
	}

	void AtlasPackerRgb::createPageData()
	{
		_pageData = new graphics::Image2dData(_pageSize * _pageSize * 3 / 4);
		_pageData->Format = graphics::Image2dFormat::Rgb;
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
		_pageData = new graphics::Image2dData(_pageSize, _pageSize);
		_pageData->Format = graphics::Image2dFormat::Rgba;
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
		_pageData = new graphics::Image2dData(_pageSize * _pageSize / 8);
		_pageData->Format = graphics::Image2dFormat::Pvrtc12;
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

}