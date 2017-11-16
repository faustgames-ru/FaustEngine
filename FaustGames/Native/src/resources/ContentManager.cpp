#include "ContentManager.h"
#include "ContentProvider.h"
#include "lpng/png.h"
#include "AtlasPacker.h"
#include "../core/HollowsAllocationPolicy.h"

#include "../../src_rectanglebinpack/GuillotineBinPack.h"

namespace resources
{
	class ContentProviderImpementation : public llge::IContentProvider
	{
	public:
		static ContentProviderImpementation Deafualt;
		
		virtual bool API_CALL existsContent(const char *name) override
		{
			return ContentProvider::existContent(name);
		}
		virtual void API_CALL openContent(const char *name) override
		{
			ContentProvider::openContent(name);
		}
		virtual int API_CALL read(void *buffer, int bytesLimit) override
		{
			return ContentProvider::read(buffer, bytesLimit);
		}
		virtual void API_CALL closeContent() override
		{
			ContentProvider::closeContent();
		}
	};

	ContentProviderImpementation ContentProviderImpementation::Deafualt;

	bool allowPack(int w, int h)
	{
		return w > 0 && h > 0 && w < 1024 && h < 1024;
	}

	std::string nilExt("nil");
	std::string texExt("tex");
	std::string pvrExt("pvr");
	std::string atcExt("atc");
	std::string pkmExt("pkm");
	std::string etcExt("etc");
	std::string dxtExt("dxt");
	
	class ReadMemoryBuffer
	{
	private:
		static int _pos;
		static void* _ptr;
	public:
		static void setup(void* ptr)
		{
			_ptr = ptr;
			_pos = 0;
		}
		static void read(png_bytep data, png_size_t length)
		{
			memcpy(data, static_cast<png_bytep>(_ptr) + _pos, length);
			_pos += length;
		}
	};

	int ReadMemoryBuffer::_pos(0);
	void* ReadMemoryBuffer::_ptr(nullptr);

	void readDataFromMemory(png_structp pngPtr, png_bytep data, png_size_t length)
	{		
		ReadMemoryBuffer::read(data, length);
	}

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

	ContentManager::ContentManager() : _image(nullptr), _isOpened(false), _platformCompressionAtlasFormat(llge::TFPvrtc14)
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
		graphics::TextureImage2d *image = graphics::TexturesPool::GetImage();// new graphics::TextureImage2d(false, true);
		entry.fileName = name;
		entry.queryFormat = format;
		entry.textureImage = image;
		_loadEntries.push_back(entry);
		return image;
	}

	graphics::TextureImage2d* ContentManager::addLoadTexture(const char* name, int w, int h, llge::TextureQueryFormat format)
	{
		IAtlasPacker * packer = queryPacker(format);
		if (packer != nullptr)
		{
			if (packer->ready() && allowPack(w, h))
			{
				AtlasImageInput input;
				input.texture = graphics::TexturesPool::GetImage();// new graphics::TextureImage2d(false, true);;
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

	ImageInfo ContentManager::loadUnregisteredTextureSize(const char *name, llge::TextureQueryFormat queryFormat)
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
					return loadUnregisteredCompressedTextureSize(path.c_str());
				}
			}
			else if (queryFormat == llge::TQFPlatformCompressed)
			{
				path[path.size() - 3] = _compressionExt[0];
				path[path.size() - 2] = _compressionExt[1];
				path[path.size() - 1] = _compressionExt[2];
				if (ContentProvider::existContent(path.c_str()))
				{
					return loadUnregisteredCompressedTextureSize(path.c_str());
				}
			}
		}
		if (!ContentProvider::existContent(name))
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
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
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}

		m_PngPtr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, 0, pngMalloc, pngFree);

		if (!m_PngPtr)
		{
			//throw ref new Exception(-1, "png_create_read_struct failed");
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}
		png_set_read_fn(m_PngPtr, 0, readData);

		m_InfoPtr = png_create_info_struct(m_PngPtr);

		if (!m_InfoPtr)
		{
			png_destroy_read_struct(&m_PngPtr, 0, 0);
			m_PngPtr = 0;
			//throw ref new Exception(-1, "png_create_info_struct failed");
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}

		if (setjmp(png_jmpbuf(m_PngPtr)))
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			//throw ref new Exception(-1, "Error during init_io");
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}

		png_set_sig_bytes(m_PngPtr, 8);
		png_read_info(m_PngPtr, m_InfoPtr);
		int w = png_get_image_width(m_PngPtr, m_InfoPtr);
		int h = png_get_image_height(m_PngPtr, m_InfoPtr);
		m_Channels = png_get_channels(m_PngPtr, m_InfoPtr);
		graphics::Image2dFormat::e imageFormat;
		switch (m_Channels)
		{
		case 3:
			imageFormat = graphics::Image2dFormat::Rgb;
			break;
		case 4:
			imageFormat = graphics::Image2dFormat::Rgba;
			break;
		default:
			imageFormat = graphics::Image2dFormat::Rgba;
			break;
		}

		ImageInfo result(w, h, imageFormat);
		png_destroy_info_struct(m_PngPtr, &m_InfoPtr);
		png_destroy_read_struct(&m_PngPtr, 0, 0);
		return result;
	}

	void ContentManager::loadPngTexture(void* data, graphics::Image2dData* _image)
	{
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
		ReadMemoryBuffer::setup(data);

		ReadMemoryBuffer::read(pngsig, PNGSIGSIZE);

		is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
		if (is_png != 0)
		{
			_image->Width = 0;
			_image->Height = 0;
			return;
		}

		m_PngPtr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, 0, pngMalloc, pngFree);

		if (!m_PngPtr)
		{
			_image->Width = 0;
			_image->Height = 0;
			return;
		}

		png_set_read_fn(m_PngPtr, 0, readDataFromMemory);

		m_InfoPtr = png_create_info_struct(m_PngPtr);

		if (!m_InfoPtr)
		{
			png_destroy_read_struct(&m_PngPtr, 0, 0);
			m_PngPtr = 0;
			//throw ref new Exception(-1, "png_create_info_struct failed");
			_image->Width = 0;
			_image->Height = 0;
			return;
		}

		if (setjmp(png_jmpbuf(m_PngPtr)))
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			//throw ref new Exception(-1, "Error during init_io");
			_image->Width = 0;
			_image->Height = 0;
			return;
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
			_image->Width = 0;
			_image->Height = 0;
			return;
		}
		int step = m_Width * m_Channels;
		int newStep = (step / 4) * 4;
		if (newStep < step)
		{
			step = newStep + 4;
		}
		
		png_bytep * row_ptrs = (png_bytep *)core::Mem::allocate(ImageMaxHeight * sizeof(png_bytep));

		_image->realloc(m_Width*m_Height);
		for (size_t i = 0; i < (size_t)m_Height; i++)
			row_ptrs[i] = (png_byte*)_image->Pixels + i*step;

		png_read_image(m_PngPtr, row_ptrs);


		png_destroy_info_struct(m_PngPtr, &m_InfoPtr);
		png_destroy_read_struct(&m_PngPtr, 0, 0);

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
				uint *row = (uint*)row_ptrs[i];
				for (size_t j = 0; j < (size_t)m_Width; j++)
				{
					if (row[j] != 0)
					{
						row[j] = graphics::Color::premul(row[j], false);
					}
				}
			}
		}
		
		core::Mem::deallocate(row_ptrs);
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

	struct CompressedTextureHeader
	{
		static int GetSize() { return 28; }
		ushort attributes;
		ushort Format;
		int CompressionPercent;
		int OriginWidth;
		int OriginHeight;
		int BorderX;
		int BorderY;
		int RawDataOffset;
	};


	void skipData(uint* &data, int bytesCount)
	{
		byte* bytes = reinterpret_cast<byte*>(data);
		bytes += bytesCount;
		data = reinterpret_cast<uint*>(bytes);
	}
	
	uint readData(uint* &data)
	{
		uint result = *data;
		data++;
		return result;
	}

	CompressedTextureHeader getCompressedTextureHeader(uint* &data)
	{
		uint* p = data;

		CompressedTextureHeader result;
		uint formatAttributes = readData(p);
		result.attributes = static_cast<ushort>(static_cast<uint>(formatAttributes) & 0xffff000000 >> 16);
		result.Format = static_cast<ushort>(static_cast<uint>(formatAttributes) & 0x0000ffff);
		result.CompressionPercent = readData(p);
		int skip = readData(p);
		skipData(p, skip);
		result.OriginWidth = readData(p);
		result.OriginHeight = readData(p);;
		result.BorderX = readData(p);;
		result.BorderY = readData(p);;
		result.RawDataOffset = p - data;
		data = p;
		return result;
	}

	ImageInfo ContentManager::loadUnregisteredCompressedTextureSize(const char* name)
	{
		ContentProvider::openContent(name);
		int size = ContentProvider::read(_image->Pixels, CompressedTextureHeader::GetSize()*2);
		ContentProvider::closeContent();
		CompressedTextureHeader header = getCompressedTextureHeader(_image->Pixels);
		return ImageInfo(header.OriginWidth, header.OriginHeight, _image->Format);
	}

	graphics::Image2dData* ContentManager::loadUnregisteredCompressedTexture(const char* name)
	{
		ContentProvider::openContent(name);
		int size = ContentProvider::read(_image->Pixels, ImageBufferSize);
		ContentProvider::closeContent();

		uint* data = _image->Pixels;
		CompressedTextureHeader header = getCompressedTextureHeader(data);

		_image->Width = header.OriginWidth;
		_image->Height = header.OriginHeight;
		_image->BlocksOrder = graphics::Image2dBlocksOrder::Normal;
		_image->BorderSize = header.BorderX;
		_image->RawDataOffset = header.RawDataOffset;
		if (header.Format == 1) // Rgba4444
		{
			_image->Format = graphics::Image2dFormat::Rgba4444;
			return _image;
		}
		if (header.Format == 2 || header.Format == 3) // Pvrtc
		{
			_image->Format = graphics::Image2dFormat::Pvrtc14;
			_image->BlocksOrder = static_cast<graphics::Image2dBlocksOrder::e>(readData(data));
			_image->RawDataOffset++;
			return _image;
		}
		if (header.Format == 3) // Pvrtc
		{
			_image->Format = graphics::Image2dFormat::Pvrtc12;
			_image->BlocksOrder = static_cast<graphics::Image2dBlocksOrder::e>(readData(data));
			_image->RawDataOffset++;
			return _image;
		}
		if (header.Format == 4) // Atc
		{
			_image->Format = graphics::Image2dFormat::Atc;
			return _image;
		}
		if (header.Format == 5) // Etc2
		{
			_image->Format = graphics::Image2dFormat::Etc2;
			return _image;
		}
		if (header.Format == 6) // dxt
		{
			_image->Format = graphics::Image2dFormat::Dxt;
			return _image;
		}
		if (header.Format == 7 || header.Format == 8 || header.Format == 9 || header.Format == 10) // Etc1
		{
			_image->Format = graphics::Image2dFormat::Etc1;
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

	llge::IContentProvider* ContentManager::getContentProvider()
	{
		return &ContentProviderImpementation::Deafualt;
	}

	llge::IContentAtlasMap * API_CALL ContentManager::getContentAtlasMap()
	{
		return nullptr;
	}

	

	void ContentManager::useCompression(llge::TextureImage2dFormat format)
	{
		_platformCompressionAtlasFormat = format;
		switch (format)
		{
		case llge::TFRgba8888: 
			_compressionExt = nilExt.c_str();
			break;			
		case llge::TFRgb888: 
			_compressionExt = nilExt.c_str();
			break;
		case llge::TFRgba4444: 
			_compressionExt = texExt.c_str();
			break;
		case llge::TFPvrtc12: 
			_compressionExt = pvrExt.c_str();
			break;
		case llge::TFPvrtc14: 
			_compressionExt = pvrExt.c_str();
			break;
		case llge::TFAtc:
			_compressionExt = atcExt.c_str();
			break;
		case llge::TFEtc2:
			_compressionExt = pkmExt.c_str();
			break;
		case llge::TFDxt:
			_compressionExt = dxtExt.c_str();
			break;
		case llge::TFEtc1:
			_compressionExt = etcExt.c_str();
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
		/*
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
		*/
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
		
		//llge::TextureImage2dFormat format = _files[id].format;
		IAtlasPacker* packer = queryPacker(queryFormat);

		if (packer != nullptr)
		{
			if (packer->ready() && allowPack(w, h))
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


	llge::TextureImage2dFormat ContentManager::getAtlasFormat(llge::TextureQueryFormat format)
	{
		switch (format)
		{
		case llge::TQFRgba8888: 
			return llge::TFRgba8888;
		case llge::TQFRgba4444: 			
			return llge::TFRgba4444;
		case llge::TQFPlatformCompressed: 
			return _platformCompressionAtlasFormat;
		}
		return llge::TFRgba8888;
	}

	IAtlasPacker* ContentManager::queryPacker(llge::TextureQueryFormat format)
	{
		//return nullptr;
		if (!_isAtlasBuilderStarted) return nullptr;
		if (format == llge::TQFNone) return nullptr;
		if (format == llge::TQFRgba8888) return nullptr;
		if (format == llge::TQFRgba4444) return nullptr;
		int packerIndex = format;
		llge::TextureImage2dFormat packersFormat = getAtlasFormat(format);
		if (_packers[packerIndex] == nullptr)
		{
			_packers[packerIndex] = AtlasPacker::create(packersFormat);
			if (_packers[packerIndex] != nullptr)
			{
				_packers[packerIndex]->startPack(ImageMaxWidth);
			}
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
