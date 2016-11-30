#include "ContentManager.h"
#include "ContentProvider.h"
#include "lpng/png.h"
#include "AtlasPacker.h"
#include "../core/HollowsAllocationPolicy.h"

#include "../../src_rectanglebinpack/GuillotineBinPack.h"

namespace resources
{

	char* nilExt("nil");
	char* texExt("tex");
	char* pvrExt("pvr");
	char* atcExt("atc");
	char* pkmExt("pkm");
	char* astcExt("ast");
	/*
	class AtlasPage
	{
	};

	class AtlasOnlinePacker : public IAtlasOnlinePacker
	{
	public:
		AtlasOnlinePacker();
		void init(int w, int h);
		virtual bool insert(const AtlasEntryInput &input, AtlasEntry &output) OVERRIDE;
		virtual void applyCurrentPage() OVERRIDE;
	private:
		float scaleX(int x);
		float scaleY(int y);
		graphics::TextureAtlasPage* cretateNextPage();
		void applyPageRect(rbp::Rect r, graphics::Image2dData* image);
		rbp::GuillotineBinPack _pack;
		std::vector<graphics::TextureAtlasPage* > _pages;
		graphics::TextureAtlasPage* _actualPage;
		int _w;
		int _h;
		graphics::Image2dData* _pageData;
	};
	
	AtlasOnlinePacker::AtlasOnlinePacker() : _actualPage(nullptr), _w(1), _h(1), _pageData(nullptr)
	{
		init(4096, 4096);
	}

	void AtlasOnlinePacker::init(int w, int h)
	{
		_pack.allowRotate = false;
		_pack.Init(w, h);
		_w = w;
		_h = h;
		_pageData = new graphics::Image2dData(w, h);
	}

	bool AtlasOnlinePacker::insert(const AtlasEntryInput& input, AtlasEntry& output)
	{
		if (input.image->Width >= _w / 2 || input.image->Height >= _h / 2)
		{
			return false;
		}
		if (_pageData == nullptr) return false;
		if (_actualPage == nullptr)
		{
			_actualPage = cretateNextPage();
		}
		rbp::Rect r = _pack.Insert(input.image->Width / 2, input.image->Height / 2, true, rbp::GuillotineBinPack::RectBestAreaFit, rbp::GuillotineBinPack::SplitMinimizeArea);
		if (r.width == 0 || r.height == 0)
		{
			applyCurrentPage();
			_actualPage = cretateNextPage();
			r = _pack.Insert(input.image->Width / 2, input.image->Height / 2, true, rbp::GuillotineBinPack::RectBestAreaFit, rbp::GuillotineBinPack::SplitMinimizeArea);
		}
		if (r.width == 0 || r.height == 0)
			return false;
		applyPageRect(r, input.image);
		_actualPage->createRect(
			scaleX(r.x),
			scaleX(r.y),
			scaleX(r.width),
			scaleX(r.height),
			input.texture);
		return true;
	}

	float AtlasOnlinePacker::scaleX(int x)
	{
		return static_cast<float>(x) / static_cast<float>(_w);
	}

	float AtlasOnlinePacker::scaleY(int y)
	{
		return static_cast<float>(y) / static_cast<float>(_h);
	}

	graphics::TextureAtlasPage* AtlasOnlinePacker::cretateNextPage()
	{
		graphics::TextureAtlasPage *actualPage = new graphics::TextureAtlasPage(true);
		actualPage->create();
		_pages.push_back(actualPage);
		_pack.Init(_w, _h);
		return actualPage;
	}

	void AtlasOnlinePacker::applyPageRect(rbp::Rect r, graphics::Image2dData* image)
	{
		int rowDstBase = r.y * _w + r.x;
		int rowSrcBase = 0;
		for (int y = 0; y < r.height; y++)
		{
			int rowDst = rowDstBase;
			int rowSrc = rowSrcBase;
			for (int x = 0; x < r.width; x++)
			{
				_pageData->Pixels[rowDst] = image->Pixels[rowSrc];
				rowDst++;
				rowSrc+=2;
			}

			rowDstBase += _w;
			rowSrcBase += image->Width * 2;
		}
	}

	void AtlasOnlinePacker::applyCurrentPage()
	{
		if (_actualPage == nullptr)
			return;
		_actualPage->setData(_pageData);
	}
	*/
	void readData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		ContentProvider::read(data, length);
	}

	void * pngMalloc(png_structp pngPtr, png_alloc_size_t size)
	{
		void* result = core::Mem::allocate(size);
		return result;
	}

	void pngFree(png_structp pngPtr, void *mem)
	{
		core::Mem::deallocate(mem);
	}

	ContentManager::ContentManager() : _image(nullptr), _isOpened(false)
	{
		_compressionExt = "pvr";
		for (int i = 0; i < llge::TFEnumSize; i++)
		{
			_packers[i] = nullptr;
		}
		_isAtlasBuilderStarted = false;
	}

	void ContentManager::cleanup()
	{
		_files.clear();
	}

	unsigned int ContentManager::registerTexture(const char *name)
	{
		unsigned int result = _files.size();
		LoadRegisrtyEntry entry;
		entry.fileName = name;
		int mipLevel = 1 << graphics::GraphicsDevice::Default.config.mipmapsLevel;
		_files.push_back(entry);
		return result;
	}

	ContentManager ContentManager::Default;

	graphics::TextureImage2d* ContentManager::addLoadTexture(const char *name, llge::TextureQueryFormat format)
	{
		LoadImageEntry entry;
		graphics::TextureImage2d *image = new graphics::TextureImage2d(false, true);
		entry.fileName = name;
		entry.queryFormat = format;
		entry.textureImage = image;
		_loadEntries.push_back(entry);
		return image;
	}

	graphics::TextureImage2d* ContentManager::addLoadTexture(const char* name, int w, int h, llge::TextureQueryFormat format)
	{
		int wLimint = ImageMaxWidth / 2 - 16;
		int hLimint = ImageMaxHeight / 2 - 16;
		IAtlasPacker * packer = nullptr;// queryPacker(format);
		if (packer != nullptr)
		{
			if (w > 0 && h > 0 && packer->ready() && w <= wLimint && h <= hLimint)
			{
				AtlasImageInput input;
				input.texture = new graphics::TextureImage2d(false, true);;
				input.width = w;
				input.height = h;
				packer->add(name, input);
				return input.texture;
			}
		}
		return addLoadTexture(name, format);
	}

	void ContentManager::addDisposeTexture(graphics::TextureImage2d *image)
	{
		_disposeEntries.push_back(image);
	}



#define PNGSIGSIZE 8

	png_bytep * m_RowPtrs = 0;

	graphics::Image2dData * ContentManager::loadTexture(int id)
	{
		const char *name = _files[id].fileName.c_str();
		return loadUnregisteredTexture(name, llge::TextureQueryFormat::TQFRgba8888);
	}

	graphics::Image2dData * ContentManager::loadUnregisteredTexture(const char *name, llge::TextureQueryFormat queryFormat)
	{
		std::string path = name;
		if (path.size() > 4)
		{
			if (queryFormat == llge::TQFRgba4444)
			{
				path[path.size() - 3] = texExt[0];
				path[path.size() - 2] = texExt[1];
				path[path.size() - 1] = texExt[2];
				if (ContentProvider::existContent(path.c_str()))
				{
					return loadUnregisteredCompressedTexture(path.c_str());
				}
			}
			else if (queryFormat == llge::TQFPlatformCompressed)
			{
				path[path.size() - 3] = _compressionExt[0];
				path[path.size() - 2] = _compressionExt[1];
				path[path.size() - 1] = _compressionExt[2];
				if (ContentProvider::existContent(path.c_str()))
				{
					return loadUnregisteredCompressedTexture(path.c_str());
				}
			}
		}
		if (!ContentProvider::existContent(name))
			return 0;
		//todo: load data from content provider

		switch (graphics::GraphicsDevice::Default.config.mipmapsLevel)
		{
		case 1:
			ContentProvider::openContent((std::string(name) + "_1").c_str());
			break;
		case 2:
			ContentProvider::openContent((std::string(name) + "_2").c_str());
			break;
		default:
			ContentProvider::openContent(name);
		}


		int m_Width;
		int m_Height;
		png_structp m_PngPtr;
		png_infop m_InfoPtr;
		png_uint_32 m_BitDepth;
		png_uint_32 m_Channels;
		png_uint_32 m_ColorType;

		png_byte pngsig[PNGSIGSIZE];
		int is_png = 0;

		//Read the 8 bytes from the stream into the sig buffer.
		ContentProvider::read(pngsig, PNGSIGSIZE);

		is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
		if (is_png != 0)
		{
			/*
			fprintf(stderr, "!png \n");
			fprintf(stderr, (char *)pngsig);
			fprintf(stderr, "\n");
			*/
			//throw ref new Exception(-1, "data is not recognized as a PNG");
			return 0;
		}

		m_PngPtr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, 0, pngMalloc, pngFree);

		if (!m_PngPtr)
		{
			//throw ref new Exception(-1, "png_create_read_struct failed");
			return 0;
		}
		png_set_read_fn(m_PngPtr, 0, readData);

		m_InfoPtr = png_create_info_struct(m_PngPtr);

		if (!m_InfoPtr)
		{
			png_destroy_read_struct(&m_PngPtr, 0, 0);
			m_PngPtr = 0;
			//throw ref new Exception(-1, "png_create_info_struct failed");
			return 0;
		}

		if (setjmp(png_jmpbuf(m_PngPtr)))
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			//throw ref new Exception(-1, "Error during init_io");
			return 0;
		}

		png_set_sig_bytes(m_PngPtr, 8);

		png_read_info(m_PngPtr, m_InfoPtr);

		m_Width = png_get_image_width(m_PngPtr, m_InfoPtr);
		m_Height = png_get_image_height(m_PngPtr, m_InfoPtr);
		m_BitDepth = png_get_bit_depth(m_PngPtr, m_InfoPtr);
		m_Channels = png_get_channels(m_PngPtr, m_InfoPtr);
		m_ColorType = png_get_color_type(m_PngPtr, m_InfoPtr);


		const unsigned int stride = png_get_rowbytes(m_PngPtr, m_InfoPtr);
		if (m_Height > ImageMaxHeight || m_Width > ImageMaxWidth)
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			//throw ref new Exception(-1, "Error during init_io");
			return 0;
		}
		//if (_useLoadRegion)
		//{
		//	int offset = _loadRegion.Y * _pageWidth + _loadRegion.X;
		//	for (size_t i = 0; i < (size_t)m_Height; i++)
		//		m_RowPtrs[i] = (png_byte*)_image->Pixels + (offset + i*_pageWidth)*m_Channels;
		//}
		//else
		{
			int step = m_Width * m_Channels;
			int newStep = (step / 4) * 4;
			if (newStep < step)
			{
				step = newStep + 4;
			}
			for (size_t i = 0; i < (size_t)m_Height; i++)
				m_RowPtrs[i] = (png_byte*)_image->Pixels + i*step;
		}

		png_read_image(m_PngPtr, m_RowPtrs);


		png_destroy_info_struct(m_PngPtr, &m_InfoPtr);
		png_destroy_read_struct(&m_PngPtr, 0, 0);

		ContentProvider::closeContent();
		_image->Width = m_Width;
		_image->Height = m_Height;
		switch (m_Channels)
		{
		case 3:
			_image->Format = graphics::Image2dFormat::Rgb;
			break;
		case 4:
			_image->Format = graphics::Image2dFormat::Rgba;
			break;
		default:
			_image->Format = graphics::Image2dFormat::Rgba;
			break;
		}

		// todo: premul param
		if (_image->Format == graphics::Image2dFormat::Rgba)
		{
			for (size_t i = 0; i < (size_t)m_Height; i++)
			{
				uint *row = (uint*)m_RowPtrs[i];
				for (size_t j = 0; j < (size_t)m_Width; j++)
				{
					if (row[j] != 0)
					{
						row[j] = graphics::Color::premul(row[j], false);
					}
				}
			}
		}

		/// todo: online pack to atlas

		return _image;
	}

	graphics::Image2dData* ContentManager::loadUnregisteredPvrTexture(const char* name)
	{
		ContentProvider::openContent(name);
		int size = ContentProvider::read(_image->Pixels, ImageBufferSize);
		ContentProvider::closeContent();
		_image->Width = *(_image->Pixels + 0);
		_image->Height = *(_image->Pixels + 1);
		_image->Border = *(_image->Pixels + 2);
		int bpp = *(_image->Pixels + 3);
		_image->Format = bpp == 2?graphics::Image2dFormat::Pvrtc12: graphics::Image2dFormat::Pvrtc14;
		_image->RawDataOffset = 5;
		return _image;
	}

	graphics::Image2dData* ContentManager::loadUnregisteredRgba4444Texture(const char* name)
	{
		ContentProvider::openContent(name);
		int size = ContentProvider::read(_image->Pixels, ImageBufferSize);
		ContentProvider::closeContent();
		_image->Width = *(_image->Pixels + 0);
		_image->Height = *(_image->Pixels + 1);
		_image->Format = graphics::Image2dFormat::Rgba4444;
		_image->RawDataOffset = 2;
		return _image;
	}

	graphics::Image2dData* ContentManager::loadUnregisteredCompressedTexture(const char* name)
	{
		ContentProvider::openContent(name);
		int size = ContentProvider::read(_image->Pixels, ImageBufferSize);
		ContentProvider::closeContent();
		int format = *(_image->Pixels + 0);
		int compression = *(_image->Pixels + 1);
		int skipHashWords = 2 + *(_image->Pixels + 2) / 4;
		_image->Width = *(_image->Pixels + 1 + skipHashWords);
		_image->Height = *(_image->Pixels + 2 + skipHashWords);
		_image->Border = 0;
		if (format == 1) // Rgba4444
		{
			_image->Format = graphics::Image2dFormat::Rgba4444;
			_image->RawDataOffset = 3 + skipHashWords;
			return _image;
		}
		if (format == 2 || format == 3) // Pvrtc
		{
			_image->Border = *(_image->Pixels + 3+ skipHashWords);
			int bpp = *(_image->Pixels + 4 + skipHashWords);
			_image->Format = bpp == 2 ? graphics::Image2dFormat::Pvrtc12 : graphics::Image2dFormat::Pvrtc14;
			_image->RawDataOffset = 6 + skipHashWords;
			return _image;
		}
		if (format == 4) // Atc
		{
			_image->Format = graphics::Image2dFormat::Atc;
			_image->RawDataOffset = 5 + skipHashWords;
			return _image;
		}
		if (format == 5) // Etc2
		{
			int hasAlpha = *(_image->Pixels + 3 + skipHashWords);

			_image->Format = hasAlpha ? graphics::Image2dFormat::Etc2 : graphics::Image2dFormat::Etc1;
			_image->RawDataOffset = 5 + skipHashWords;
			return _image;
		}
		if (format == 6) // Astc
		{
			int hasAlpha = *(_image->Pixels + 3 + skipHashWords);

			_image->Format = graphics::Image2dFormat::Astc;
			_image->RawDataOffset = 5 + skipHashWords;
			return _image;
		}
		return nullptr;
	}

	char* ContentManager::loadString(const char* name)
	{
		ContentProvider::openContent(name);
		char * string = static_cast<char *>(getBuffer());
		int len = 0;
		const int pageSize = 256 * 1024;
		int count = 0;
		while ((count = resources::ContentProvider::read(string + len, pageSize)) > 0)
		{
			len += count;
		}
		string[len] = 0;
		ContentProvider::closeContent();
		return string;
	}

	void ContentManager::open()
	{
		if (_isOpened)
			return;
		m_RowPtrs = (png_bytep *)core::Mem::allocate(ImageMaxHeight * sizeof(png_bytep));
		_image = new graphics::Image2dData(ImageBufferSize);
		_isOpened = true;
	}
	void ContentManager::close()
	{
		if (!_isOpened)
			return;
		core::Mem::deallocate(m_RowPtrs);
		delete _image;
		m_RowPtrs = 0;
		_image = 0;
		_isOpened = false;
		//_packerRGBA->applyCurrentPage();
	}

	void ContentManager::startAtlasBuild()
	{		
		AtlasTexturesPool::Default.clear();
		for (int i = 0; i < llge::TFEnumSize; i++)
		{
			IAtlasPacker* packer = _packers[i];
			if (packer != nullptr)
				packer->startPack(ImageMaxWidth);
		}
		_isAtlasBuilderStarted = true;		
	}

	void ContentManager::finishAtlasBuild()
	{		
		for (int i = 0; i < llge::TFEnumSize; i++)
		{
			IAtlasPacker* packer = _packers[i];
			if (packer != nullptr)
			{
				packer->finishPack();
				packer->loadFiles();
			}
		}
		_isAtlasBuilderStarted = false;		
	}

	llge::IContentAtlasMap * API_CALL ContentManager::getContentAtlasMap()
	{
		return nullptr;
	}

	

	void ContentManager::useCompression(llge::TextureImage2dFormat format)
	{
		switch (format)
		{
		case llge::TFRgba8888: 
			_compressionExt = nilExt;
			break;			
		case llge::TFRgb888: 
			_compressionExt = nilExt;
			break;
		case llge::TFRgba4444: 
			_compressionExt = texExt;
			break;
		case llge::TFPvrtc12: 
			_compressionExt = pvrExt;
			break;
		case llge::TFPvrtc14: 
			_compressionExt = pvrExt;
			break;
		case llge::TFAtc:
			_compressionExt = atcExt;
			break;
		case llge::TFEtc2:
			_compressionExt = pkmExt;
			break;
		case llge::TFAstc:
			_compressionExt = astcExt;
			break;
		default: break;
		}
	}

	void API_CALL ContentManager::replaceSeparator(bool value)
	{
		_replaceSeparator = value;
	}

	int API_CALL ContentManager::registerImage(char * name)
	{
		return registerTexture(name);
	}

	void API_CALL ContentManager::startLoad()
	{
		open();
	}

	bool API_CALL ContentManager::update()
	{
		if(!ImageSizeLoaded)
		{
			int maxTextureSize[1];
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxTextureSize);
			if (glGetError() == GL_NO_ERROR)
			{
				int size = maxTextureSize[0];
				const int sizeLimit = 1024 * 4;
				if (size > sizeLimit)
					size = sizeLimit;
				ImageMaxWidth = size;
				ImageMaxHeight = size;
				ImageBufferSize = ImageMaxWidth*ImageMaxHeight;
				ImageSizeLoaded = true;
			}
		}

		if ((_loadEntries.size() == 0) && (_disposeEntries.size() == 0))
			return true;

		bool wasOpened = _isOpened;

		if (!_isOpened)
			open();


		for (uint i = 0; i < _loadEntries.size(); i++)
		{
			/*
			fprintf(stderr, _loadEntries[i].fileName.c_str());
			fprintf(stderr, "\n");
			*/
			graphics::Image2dData * image = loadUnregisteredTexture(_loadEntries[i].fileName.c_str(), _loadEntries[i].queryFormat);
			_loadEntries[i].textureImage->create();
			if (image != nullptr)
			{
				_loadEntries[i].textureImage->setData(image);
			}
		}
		_loadEntries.clear();
		for (uint i = 0; i < _disposeEntries.size(); i++)
		{
			_disposeEntries[i]->cleanup();
			delete _disposeEntries[i];
		}
		_disposeEntries.clear();

		if (!wasOpened)
			close();
		return false;
	}


	void API_CALL ContentManager::loadImage(int id, llge::ITextureImage2d *textureImage, int w, int h, llge::TextureQueryFormat queryFormat)
	{
		const char *name = _files[id].fileName.c_str();
		graphics::TextureImage2d* texture = static_cast<graphics::TextureImage2d*>(textureImage->getTextureImageInstance());
		
		int wLimint = ImageMaxWidth * 2 / 3;
		int hLimint = ImageMaxHeight * 2 / 3;

		//llge::TextureImage2dFormat format = _files[id].format;
		IAtlasPacker* packer = nullptr;// queryPacker(format);

		if (packer != nullptr)
		{
			if (w > 0 && h > 0 && packer->ready() && w < wLimint && h < hLimint)
			{
				AtlasImageInput input;
				input.texture = texture;
				input.width = w;
				input.height = h;
				packer->add(name, input);
				return;
			}
		}
		graphics::Image2dData * image = loadUnregisteredTexture(name, queryFormat);

		if (image)
		{
			texture->create();
			graphics::TextureImage2d* textureInstance = static_cast<graphics::TextureImage2d*>(textureImage->getTextureImageInstance());
			textureInstance->setData(image);
		}
	}

	llge::ITextureBuffer2d * API_CALL ContentManager::loadBuffer(int id)
	{
		loadTexture(id);
		return this;
	}

	llge::TextureImage2dFormat API_CALL ContentManager::getFormat()
	{
		return (llge::TextureImage2dFormat)_image->Format;
	}

	int API_CALL ContentManager::getWidth()
	{
		return _image->Width;
	}

	int API_CALL ContentManager::getHeight()
	{
		return _image->Height;
	}

	IntPtr API_CALL ContentManager::getPixels()
	{
		return _image->Pixels;
	}
	
	void* ContentManager::getBuffer() const
	{
		if (_image == 0)
			return 0;
		return _image->Pixels;
	}

	int ContentManager::getBufferSize() const
	{
		if (_image == 0)
			return 0;
		return ImageBufferSize;
	}

	IAtlasPacker* ContentManager::queryPacker(llge::TextureImage2dFormat format)
	{
		return nullptr;
		if (!_isAtlasBuilderStarted) return nullptr;
		if (format != llge::TextureImage2dFormat::TFRgba8888)return nullptr;
		int packerIndex = llge::TextureImage2dFormat::TFRgba8888;// format;
		if (_packers[packerIndex] == nullptr)
		{
			_packers[packerIndex] = AtlasPacker::create(llge::TextureImage2dFormat::TFRgba8888);
			_packers[packerIndex]->startPack(ImageMaxWidth);
		}
		return _packers[packerIndex];
		
	}

	void API_CALL ContentManager::finishLoad()
	{
		close();
	}

	void API_CALL ContentManager::dispose()
	{
		delete this;
	}

	bool ContentManager::_replaceSeparator(false);

	bool ContentManager::ImageSizeLoaded(false);

	int ContentManager::ImageBufferSize = 2048 * 2048;
	int ContentManager::ImageMaxHeight = 2048;
	int ContentManager::ImageMaxWidth = 2048;
}