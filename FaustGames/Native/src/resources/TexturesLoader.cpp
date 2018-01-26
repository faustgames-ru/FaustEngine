#include "TexturesLoader.h"
#include "ContentProvider.h"
#include "lpng/png.h"
#include "LoadingBuffer.h"

#define PNGSIGSIZE 8

namespace resources
{
	void texturesLoaderPngReadData(png_structp pngPtr, png_bytep data, png_size_t length);
	void * texturesLoaderPngMalloc(png_structp pngPtr, png_alloc_size_t size);
	void texturesLoaderPngFree(png_structp pngPtr, void *mem);

	TexturesLoader TexturesLoader::Default;
	ReadService* TexturesLoader::ReadService(nullptr);
	ReadMemoryService TexturesLoader::ReadMemoryService;
	ReadFileService TexturesLoader::ReadFileService;


	ReadService::~ReadService()
	{
	}

	ReadMemoryService::ReadMemoryService()
	{
		_ptr = nullptr;
		_pos = 0;
	}

	void ReadMemoryService::setup(void* data)
	{
		_ptr = data;
		_pos = 0;
	}

	void ReadMemoryService::read(void* data, int size)
	{
		memcpy(data, static_cast<png_bytep>(_ptr) + _pos, size);
		_pos += size;
	}

	void ReadFileService::read(void* data, int size)
	{
		ContentProvider::read(data, size);
	}

	TexturesLoader::TexturesLoader()
	{
		_compressionFormat = llge::TFRgba8888;
		_loadingBuffer = nullptr;
		_loadingBufferSize = 0;
		_loadingHeaderBuffer = new byte[CompressedTextureHeader::GetSize() * 2];
	}

	void TexturesLoader::useCompression(llge::TextureImage2dFormat format)
	{
		_compressionFormat = format;
		switch (_compressionFormat)
		{
		case llge::TFRgba8888:
		case llge::TFRgb888:
			_compressionExt = pngExt;
			break;
		case llge::TFRgba4444:
			_compressionExt = texExt;
			break;
		case llge::TFPvrtc12:
		case llge::TFPvrtc14:
			_compressionExt = pvrExt;
			break;
		case llge::TFAtc:
			_compressionExt = atcExt;
			break;
		case llge::TFEtc2:
			_compressionExt = pkmExt;
			break;
		case llge::TFDxt:
			_compressionExt = dxtExt;
			break;
		case llge::TFEtc1:
			_compressionExt = etcExt;
			break;
		default: 
			_compressionExt = nilExt;
			break;
		}
	}

	ImageInfo TexturesLoader::loadTextureSize(const char* name, llge::TextureQueryFormat queryFormat)
	{
		auto fileName = getFileName(name, queryFormat);
		if (!ContentProvider::existContent(fileName.c_str()))
		{
			return emptyImageInfo();
		}
		if (equalsExt(fileName, pngExt))
		{
			return loadPngTextureSize(fileName.c_str());
		}
		return loadCompressedTextureSize(fileName.c_str());
	}

	graphics::Image2dResourceData TexturesLoader::readImage(const char* name, llge::TextureQueryFormat queryFormat)
	{
		auto fileName = getFileName(name, queryFormat);
		if (!ContentProvider::existContent(fileName.c_str()))
		{
			return emptyImage();
		}
		if (equalsExt(fileName, pngExt))
		{
			return loadPngTexture(fileName.c_str());
		}
		return loadCompressedTexture(fileName.c_str());
	}

	graphics::Image2dResourceData TexturesLoader::emptyImage()
	{
		graphics::Image2dResourceData result;
		result.Width = 1;
		result.Height = 1;
		result.BlocksOrder = graphics::Image2dBlocksOrder::Normal;
		result.BorderSize = 0;
		result.Format = graphics::Image2dFormat::Rgba;
		result.RawDataOffset = 0;
		result.Pixels = &emptyPixel;
		return result;
	}

	ImageInfo TexturesLoader::emptyImageInfo()
	{
		return ImageInfo(1, 1, graphics::Image2dFormat::Rgba);
	}

	void TexturesLoader::clearLoadingBuffer()
	{
		if (_loadingBuffer == nullptr) return;
		delete[] _loadingBuffer;
		_loadingBufferSize = 0;
	}

	void TexturesLoader::resizeLoadingBuffer(int size)
	{
		if (_loadingBufferSize >= size) return;
		int bufferSize = _loadingBufferSize;
		clearLoadingBuffer();
		if (bufferSize == 0)
		{
			bufferSize = 1;
		}
		while (bufferSize < size)
		{
			bufferSize *= 2;
		}
		_loadingBuffer = new byte[bufferSize];
	}

	graphics::Image2dFormat::e HeaderFormatToGraphicsFormat(ushort format)
	{
		if (format == 1) // Rgba4444
		{
			return graphics::Image2dFormat::Rgba4444;
		}
		if (format == 2 || format == 3) // Pvrtc
		{
			return graphics::Image2dFormat::Pvrtc14;
		}
		if (format == 3) // Pvrtc
		{
			return  graphics::Image2dFormat::Pvrtc12;
		}
		if (format == 4) // Atc
		{
			return  graphics::Image2dFormat::Atc;
		}
		if (format == 5) // Etc2
		{
			return graphics::Image2dFormat::Etc2;
		}
		if (format == 6) // dxt
		{
			return graphics::Image2dFormat::Dxt;
		}
		if (format == 7 || format == 8 || format == 9 || format == 10) // Etc1
		{
			return graphics::Image2dFormat::Etc1;
		}

		return graphics::Image2dFormat::Rgba;
	}


	graphics::Image2dResourceData TexturesLoader::loadCompressedTexture(const char* name)
	{
		ContentProvider::openContent(name);
		int size = ContentProvider::ContentProviderInstance->getContentSize();
		resizeLoadingBuffer(size);
		int readSize = ContentProvider::read(_loadingBuffer, size);
		ContentProvider::closeContent();
		return loadCompressedTexture(_loadingBuffer, readSize);
	}

	graphics::Image2dResourceData TexturesLoader::loadCompressedTexture(void* memoryBuffer, int size) const
	{
		LoadingBuffer buffer(memoryBuffer, size);
		CompressedTextureHeader header = buffer.readCompressedHeader();
		graphics::Image2dResourceData result;

		result.Width = header.OriginWidth;
		result.Height = header.OriginHeight;
		result.BlocksOrder = graphics::Image2dBlocksOrder::Normal;
		result.BorderSize = header.BorderX;
		result.RawDataOffset = header.RawDataOffset;
		result.Pixels = reinterpret_cast<uint*>(_loadingBuffer);
		if (header.Format == 1) // Rgba4444
		{
			result.Format = graphics::Image2dFormat::Rgba4444;
			return result;
		}
		if (header.Format == 2 || header.Format == 3) // Pvrtc
		{
			result.Format = graphics::Image2dFormat::Pvrtc14;
			result.BlocksOrder = static_cast<graphics::Image2dBlocksOrder::e>(buffer.readInt());
			result.RawDataOffset++;
			return result;
		}
		if (header.Format == 3) // Pvrtc
		{
			result.Format = graphics::Image2dFormat::Pvrtc12;
			result.BlocksOrder = static_cast<graphics::Image2dBlocksOrder::e>(buffer.readInt());
			result.RawDataOffset++;
			return result;
		}
		if (header.Format == 4) // Atc
		{
			result.Format = graphics::Image2dFormat::Atc;
			return result;
		}
		if (header.Format == 5) // Etc2
		{
			result.Format = graphics::Image2dFormat::Etc2;
			return result;
		}
		if (header.Format == 6) // dxt
		{
			result.Format = graphics::Image2dFormat::Dxt;
			return result;
		}
		if (header.Format == 7 || header.Format == 8 || header.Format == 9 || header.Format == 10) // Etc1
		{
			result.Format = graphics::Image2dFormat::Etc1;
			return result;
		}
		// todo: loading image 
		return result;
	}

	ImageInfo TexturesLoader::loadCompressedTextureSize(const char* name) const
	{
		int headerSize = CompressedTextureHeader::GetSize() * 2;
		ContentProvider::openContent(name);
		ContentProvider::read(_loadingHeaderBuffer, headerSize);
		ContentProvider::closeContent();

		return loadCompressedTextureSize(_loadingHeaderBuffer, headerSize);
	}

	ImageInfo TexturesLoader::loadCompressedTextureSize(void* memoryBuffer, int size)
	{
		LoadingBuffer buffer(memoryBuffer, size);

		CompressedTextureHeader header = buffer.readCompressedHeader();
		auto result = ImageInfo(header.OriginWidth, header.OriginHeight, HeaderFormatToGraphicsFormat(header.Format));
		result.compressionHeader = header;
		return result;
	}

	std::string TexturesLoader::getFileName(const char* name, llge::TextureQueryFormat queryFormat)
	{
		std::string fileName = name;
		switch (queryFormat)
		{
		case llge::TQFRgba4444:
			changeExt(fileName, texExt);
			break;
		case llge::TQFPlatformCompressed:
			changeExt(fileName, _compressionExt);
			break;
		default:
			changeExt(fileName, pngExt);
			break;
		}
		if(!ContentProvider::existContent(fileName.c_str()))
		{
			changeExt(fileName, pngExt);
		}
		return fileName;;
	}

	void TexturesLoader::changeExt(std::string& str, const std::string& ext)
	{
		if (str.size() < 4) return;
		str[str.size() - 3] = ext[0];
		str[str.size() - 2] = ext[1];
		str[str.size() - 1] = ext[2];
	}

	bool TexturesLoader::equalsExt(const std::string& str, const std::string& ext)
	{
		if (str.size() < 5) return false;
		if (str[str.size() - 4] != '.') return false;
		if (str[str.size() - 3] != ext[0]) return false;
		if (str[str.size() - 2] != ext[1]) return false;
		if (str[str.size() - 1] != ext[2]) return false;
	}

	std::string TexturesLoader::nilExt("nil");
	std::string TexturesLoader::pngExt("png");
	std::string TexturesLoader::texExt("tex");
	std::string TexturesLoader::pvrExt("pvr");
	std::string TexturesLoader::atcExt("atc");
	std::string TexturesLoader::pkmExt("pkm");
	std::string TexturesLoader::etcExt("etc");
	std::string TexturesLoader::dxtExt("dxt");

	unsigned int TexturesLoader::emptyPixel(0x00000000);
	
	ImageInfo TexturesLoader::loadPngTextureSize(const char* name)
	{
		ContentProvider::openContent(name);

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
			ContentProvider::closeContent();
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}

		m_PngPtr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, 0, texturesLoaderPngMalloc, texturesLoaderPngFree);

		if (!m_PngPtr)
		{
			ContentProvider::closeContent();
			//throw ref new Exception(-1, "png_create_read_struct failed");
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}
		png_set_read_fn(m_PngPtr, 0, texturesLoaderPngReadData);

		m_InfoPtr = png_create_info_struct(m_PngPtr);

		if (!m_InfoPtr)
		{
			png_destroy_read_struct(&m_PngPtr, 0, 0);
			m_PngPtr = 0;
			ContentProvider::closeContent();
			//throw ref new Exception(-1, "png_create_info_struct failed");
			return ImageInfo(0, 0, graphics::Image2dFormat::Rgba);
		}

		if (setjmp(png_jmpbuf(m_PngPtr)))
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			ContentProvider::closeContent();
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

		ContentProvider::closeContent();

		ImageInfo result(w, h, imageFormat);
		png_destroy_info_struct(m_PngPtr, &m_InfoPtr);
		png_destroy_read_struct(&m_PngPtr, 0, 0);

		return result;
	}

	graphics::Image2dResourceData TexturesLoader::loadPngTexture()
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
		ReadService->read(pngsig, PNGSIGSIZE);
		is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
		if (is_png != 0)
		{
			/*
			fprintf(stderr, "!png \n");
			fprintf(stderr, (char *)pngsig);
			fprintf(stderr, "\n");
			*/
			//throw ref new Exception(-1, "data is not recognized as a PNG");
			return emptyImage();
		}

		m_PngPtr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, 0, texturesLoaderPngMalloc, texturesLoaderPngFree);

		if (!m_PngPtr)
		{
			//throw ref new Exception(-1, "png_create_read_struct failed");			
			return emptyImage();;
		}
		png_set_read_fn(m_PngPtr, 0, texturesLoaderPngReadData);

		m_InfoPtr = png_create_info_struct(m_PngPtr);

		if (!m_InfoPtr)
		{
			png_destroy_read_struct(&m_PngPtr, 0, 0);
			m_PngPtr = 0;
			//throw ref new Exception(-1, "png_create_info_struct failed");			
			return emptyImage();
		}

		if (setjmp(png_jmpbuf(m_PngPtr)))
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			//throw ref new Exception(-1, "Error during init_io");
			return emptyImage();
		}

		png_set_sig_bytes(m_PngPtr, 8);

		png_read_info(m_PngPtr, m_InfoPtr);

		m_Width = png_get_image_width(m_PngPtr, m_InfoPtr);
		m_Height = png_get_image_height(m_PngPtr, m_InfoPtr);
		m_BitDepth = png_get_bit_depth(m_PngPtr, m_InfoPtr);
		m_Channels = png_get_channels(m_PngPtr, m_InfoPtr);
		m_ColorType = png_get_color_type(m_PngPtr, m_InfoPtr);


		const unsigned int stride = png_get_rowbytes(m_PngPtr, m_InfoPtr);
		/*
		if (m_Height > ImageMaxHeight || m_Width > ImageMaxWidth)
		{
			png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);
			//throw ref new Exception(-1, "Error during init_io");
			return 0;
		}
		*/
		//if (_useLoadRegion)
		//{
		//	int offset = _loadRegion.Y * _pageWidth + _loadRegion.X;
		//	for (size_t i = 0; i < (size_t)m_Height; i++)
		//		m_RowPtrs[i] = (png_byte*)_image->Pixels + (offset + i*_pageWidth)*m_Channels;
		//}
		//else

		int step = m_Width * m_Channels;
		int newStep = (step / 4) * 4;
		if (newStep < step)
		{
			step = newStep + 4;
		}
		
		resizeLoadingBuffer(step*m_Height);

		png_byte** m_RowPtrs = new png_byte*[m_Height]; // todo: use loading buffer

		for (size_t i = 0; i < static_cast<size_t>(m_Height); i++)
			m_RowPtrs[i] = static_cast<png_byte*>(_loadingBuffer) + i*step;

		png_read_image(m_PngPtr, m_RowPtrs);


		png_destroy_info_struct(m_PngPtr, &m_InfoPtr);
		png_destroy_read_struct(&m_PngPtr, 0, 0);

		graphics::Image2dResourceData image;

		image.Width = m_Width;
		image.Height = m_Height;
			
		switch (m_Channels)
		{
		case 3:
			image.Format = graphics::Image2dFormat::Rgb;
			break;
		case 4:
			image.Format = graphics::Image2dFormat::Rgba;
			break;
		default:
			image.Format = graphics::Image2dFormat::Rgba;
			break;
		}

		image.BlocksOrder = graphics::Image2dBlocksOrder::Normal;
		image.BorderSize = 0;
		image.RawDataOffset = 0;
		image.Pixels = reinterpret_cast<uint* >(_loadingBuffer);

		// todo: premul param
		if (image.Format == graphics::Image2dFormat::Rgba)
		{
			for (size_t i = 0; i < (size_t)m_Height; i++)
			{
				uint *row = (uint*)m_RowPtrs[i];
				for (size_t j = 0; j < (size_t)m_Width; j++)
				{
					if (row[j] != 0)
					{
						row[j] = graphics::Color::premul(row[j], graphics::BlendState::Alpha);
					}
				}
			}
		}

		delete[] m_RowPtrs;

		return image;
	}

	graphics::Image2dResourceData TexturesLoader::loadPngTexture(const char* name)
	{
		ContentProvider::openContent(name);
		ReadService = &ReadFileService;
		graphics::Image2dResourceData result = loadPngTexture();
		ContentProvider::closeContent();	
		ReadService = nullptr;
		return result;
	}

	graphics::Image2dResourceData TexturesLoader::loadPngTexture(void* buffer)
	{
		ReadMemoryService.setup(buffer);
		ReadService = &ReadMemoryService;
		graphics::Image2dResourceData result = loadPngTexture();
		ReadService = nullptr;
		return result;
	}


	void texturesLoaderPngReadData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		TexturesLoader::ReadService->read(data, length);
	}

	void * texturesLoaderPngMalloc(png_structp pngPtr, png_alloc_size_t size)
	{
		void* result = core::Mem::allocate(size);
		return result;
	}

	void texturesLoaderPngFree(png_structp pngPtr, void *mem)
	{
		core::Mem::deallocate(mem);
	}

}
