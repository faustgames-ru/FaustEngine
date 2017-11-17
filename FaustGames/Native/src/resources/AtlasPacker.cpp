#include "AtlasPacker.h"

#include "ContentManager.h"

namespace resources
{
	BinPackRect BinPackRect::empty;
	
	BinPackRect::BinPackRect()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	int BinPackRect::square()
	{
		return width*height;
	}

	void BinPackRect::divideX(int w, BinPackRect& left, BinPackRect& right)
	{
		left = *this;
		right = *this;
		left.width = w;
		right.width = width - w;
		right.x = x + w;
	}

	void BinPackRect::divideY(int h, BinPackRect& top, BinPackRect& bottom)
	{
		top = *this;
		bottom = *this;
		top.height = h;
		bottom.height = height - h;
		bottom.y = y + h;
	}

	bool BinPackRect::contains(int w, int h)
	{
		return w <= width && h <= height;
	}

	void BinPackNode::construct()
	{
		rect = BinPackRect::empty;
		inserted = BinPackRect::empty;
		userData = nullptr;
		childs[0] = nullptr;
		childs[1] = nullptr;
	}

	BinPackNode* BinPackNode::insert(BinPackNodesPool* pool, int w, int h)
	{
		if (childs[0] == nullptr)
		{
			childs[0] = pool->createNode();
			childs[1] = pool->createNode();

			BinPackRect l, r, t, b;

			rect.divideX(w, l, r);
			rect.divideY(h, t, b);

			if (r.square() > b.square())
			{
				l.divideY(h, inserted, b);
				childs[0]->rect = b;
				childs[1]->rect = r;
			}
			else
			{
				t.divideX(w, inserted, r);
				childs[0]->rect = r;
				childs[1]->rect = b;
			}
			return this;
		}
		BinPackNode* result = nullptr;
		if (childs[0]->rect.contains(w, h))
		{
			result =  childs[0]->insert(pool, w, h);
		}
		if (result != nullptr)
		{
			return result;
		}
		if (childs[1]->rect.contains(w, h))
		{
			result = childs[1]->insert(pool, w, h);
		}
		return result;
	}

	BinPackNodesBlock::BinPackNodesBlock() : _count(0)
	{
	}

	void BinPackNodesBlock::clear()
	{
		_count = 0;
	}

	BinPackNode* BinPackNodesBlock::queryNew()
	{
		if (_count == MaxCount) 
			return nullptr;
		BinPackNode* result = _nodes + _count;
		result->construct();
		_count++;
		return result;
	}


	BinPackNodesPool::BinPackNodesPool(): _blockIndex(0)
	{
		_blocks.push_back(new BinPackNodesBlock());
	}

	BinPackNode* BinPackNodesPool::createNode()
	{
		BinPackNode* result = _blocks[_blockIndex]->queryNew();
		if (result == nullptr)
		{
			_blocks.push_back(new BinPackNodesBlock());
			_blockIndex++;
			result = _blocks[_blockIndex]->queryNew();
		}
		return result;
	}

	void BinPackNodesPool::clear()
	{
		for (int i = 0; i < _blocks.size(); i++)
		{
			_blocks[i]->clear();
		}
		_blockIndex = 0;
	}

	BinPack BinPack::Deafult;

	BinPack::BinPack(): _width(0), _height(0), _root(nullptr)
	{
	}

	void BinPack::clear(int w, int h)
	{
		_width = w;
		_height = h;
		_root = nullptr;
		_pool.clear();
	}

	BinPackNode* BinPack::insert(int w, int h)
	{
		if (_root == nullptr)
		{
			_root = _pool.createNode();
			_root->rect.width = _width;
			_root->rect.height = _height;
		};
		if (!_root->rect.contains(w, h))
			return nullptr;
		return _root->insert(&_pool, w, h);
	}

	AtlasImageEntry::AtlasImageEntry(const char* p, const AtlasImageInput& e)
	{
		path = p;
		input = e;
	}

	AtlasRect::AtlasRect(BinPackRect r, AtlasImageEntry* e)
	{
		rect = r;
		entry = e;
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

	bool compareRectSize(const RectSize &i, const RectSize &j)
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

	void AtlasPacker::loadFiles()
	{
		releasePages();
		std::vector<RectSize> rects;
		for (uint i = 0; i < _inputPack.size(); i++)
		{
			RectSize size;
			
			//ImageInfo imageInfo = ContentManager::Default.loadUnregisteredTextureSize(_inputPack[i]->path.c_str(), getQueryFormat());
            
            //size.width = alignInfo.alignWidth(imageInfo.Width);
            //size.height = alignInfo.alignHeight(imageInfo.Height);
            
			size.entry = _inputPack[i];
			size.width = _inputPack[i]->input.width;
			size.height = _inputPack[i]->input.height;
			rects.push_back(size);
		}

		sort(rects.begin(), rects.end(), compareRectSize);
		
		int startIndex = 0;
		while (startIndex < rects.size())
		{
			BinPack::Deafult.clear(alignInfo.alignPageWidth(pageSize), alignInfo.alignPageHeight(pageSize));
			AtlasPage* page = new AtlasPage();
			_pages.push_back(page);
			bool wasBreak = false;
			for (uint i = startIndex; i < rects.size(); i++)
			{
				BinPackNode* res = BinPack::Deafult.insert(rects[i].width, rects[i].height);
				if (res == nullptr)
				{
					startIndex = i;
					wasBreak = true;
					break;
				}
				page->rects.push_back(AtlasRect(res->inserted, rects[i].entry));
			}
			if (!wasBreak)
			{
				startIndex = rects.size();
			}
		}


		if (_pageData == nullptr)
		{
			_pageData = new graphics::Image2dData(placer->getPageBufferSize(pageSize));
			_pageData->BlocksOrder = placer->getPageBlocksOrder();
			_pageData->Format = getFormat();
			_pageData->Width = pageSize;
			_pageData->Height = pageSize;
			_pageData->RawDataOffset = 0;
		}
		PlaceArgs placeArgs;
		placeArgs.pageData = _pageData;
		// todo: cerate pages textures
		for (uint k = 0; k < _pages.size(); k++)
		{
			AtlasPage* page = _pages[k];
            graphics::TextureAtlasPage* texture = AtlasTexturesPool::Default.queryPage();
			if (_pageData->Format == graphics::Image2dFormat::Etc1)
			{
				texture->createAlphaIfNeeded();
			}
			for (uint i = 0; i < page->rects.size(); i++)
			{
				AtlasRect rect = page->rects[i];
				
				graphics::Image2dData* image = ContentManager::Default.loadUnregisteredTexture(page->rects[i].entry->path.c_str(), getQueryFormat());
                if (image == nullptr)
                {
                    std::string path =page->rects[i].entry->path.c_str();
                    continue;
                }
                
				llge::TextureImage2dFormat format = graphics::Image2dFormat::ToLlgeFormat(image->Format);
				if (format != _internalFormat)
				{
					rect.entry->input.texture->AtlasEntry = false;
					rect.entry->input.texture->create();
					rect.entry->input.texture->setData(image);
					continue;
				}
				
				int inputW = alignInfo.alignWidth(image->Width);
				int inputH = alignInfo.alignHeight(image->Height);

				if (inputW == rect.rect.width && inputH == rect.rect.height)
				{
					if (placer == nullptr) continue;
					placeArgs.alignInfo = alignInfo;
					placeArgs.rect = rect.rect;
					placeArgs.imageData = image;

					placer->placeImage(placeArgs);
					 
					rect.entry->input.texture->associate(texture);
					rect.entry->input.texture->transform = graphics::TextureTransform(
						static_cast<float>((rect.rect.x + alignInfo.borderBlockCount)*alignInfo.blockSizeX) / static_cast<float>(pageSize),
						static_cast<float>((rect.rect.y + alignInfo.borderBlockCount)*alignInfo.blockSizeY) / static_cast<float>(pageSize),
						static_cast<float>(image->Width / static_cast<float>(pageSize)),
						static_cast<float>(image->Height / static_cast<float>(pageSize)));
				}				
				else 
				{
					rect.entry->input.texture->AtlasEntry = false;
					rect.entry->input.texture->create();
					rect.entry->input.texture->setData(image);
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
		result.height = alignInfo.alignHeight(result.height);
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
		case llge::TFDxt:
			return graphics::Image2dFormat::Dxt;
		case llge::TFEtc1:
			return graphics::Image2dFormat::Etc1;
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
	IAtlasPlacer* IAtlasPlacer::atc = new AtlasPlacerAtc();
	IAtlasPlacer* IAtlasPlacer::etc2 = new AtlasPlacerEtc2();
	IAtlasPlacer* IAtlasPlacer::etc1 = new AtlasPlacerEtc1();
	IAtlasPlacer* IAtlasPlacer::dxt = new AtlasPlacerDxt();
	

	IAtlasPlacer* IAtlasPlacer::switchPlacer(llge::TextureImage2dFormat format)
	{
		switch (format)
		{
		case llge::TFRgba8888:
			return rgba8888;
		case llge::TFRgba4444:
			return nullptr;// rgba4444;
		case llge::TFPvrtc14:
			return pvrtc14;
		case llge::TFAtc:
			return atc;
		case llge::TFEtc2:
			return etc2;
		case llge::TFEtc1:
			return etc1;
		case llge::TFDxt:
			return dxt;
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
			result = graphics::TexturesPool::GetAtlasPage();// new graphics::TextureAtlasPage(true);
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
		AbstractPlacer::placeImageWithBorderMortonOrder<int64_t>(e);
	}

	void AtlasPlacerPvrtc14::SetupAlign(AlignInfo& alignInfo)
	{
		alignInfo.blockSizeX = 4;
		alignInfo.blockSizeY = 4;
		alignInfo.borderBlockCount = 1;
	}

	graphics::Image2dBlocksOrder::e AtlasPlacerPvrtc14::getPageBlocksOrder()
	{
		return graphics::Image2dBlocksOrder::Morton;
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

	int AtlasPlacerDxt::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 4;
	}

	void AtlasPlacerDxt::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImageWithBorder<int128>(e);
	}

	void AtlasPlacerDxt::SetupAlign(AlignInfo& alignInfo)
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

	int AtlasPlacerEtc1Internal::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 8;
	}

	void AtlasPlacerEtc1Internal::placeImage(const PlaceArgs& e)
	{
		AbstractPlacer::placeImageWithBorder<int64_t>(e);
	}

	void AtlasPlacerEtc1Internal::SetupAlign(AlignInfo& alignInfo)
	{
		alignInfo.blockSizeX = 4;
		alignInfo.blockSizeY = 4;
		alignInfo.borderBlockCount = 1;
	}

	int AtlasPlacerEtc1::getPageBufferSize(int pageSize)
	{
		return pageSize*pageSize / 4;
	}

	void AtlasPlacerEtc1::placeImage(const PlaceArgs& e)
	{
		_rgb.placeImage(e);
		
		uint* pixelsPage = e.pageData->Pixels;
		int offsetPage = e.pageData->getFullWidth()*e.pageData->getFullHeight() / 2;
		e.pageData->Pixels = reinterpret_cast<uint*>(reinterpret_cast<byte *>(pixelsPage) + offsetPage);
		
		uint* pixelsImage = e.imageData->Pixels;
		int offsetImage = graphics::TextureImage2d::getSize(e.imageData->getFullWidth(), e.imageData->getFullHeight(), graphics::Image2dFormat::Etc1);
		e.imageData->Pixels = reinterpret_cast<uint*>(reinterpret_cast<byte *>(pixelsImage) + offsetImage);

		_a.placeImage(e);

		e.pageData->Pixels = pixelsPage;
		e.imageData->Pixels = pixelsImage;
	}

	void AtlasPlacerEtc1::SetupAlign(AlignInfo& alignInfo)
	{
		_rgb.SetupAlign(alignInfo);
		_a.SetupAlign(alignInfo);
	}
}
