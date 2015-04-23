#include "ContentManager.h"
#include "ContentProvider.h"
#include "lpng\png.h"
#include "..\core\HollowsAllocationPolicy.h"

namespace resources
{
	void readData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		ContentProvider::read(data, length);
	}

	void * pngMalloc(png_structp pngPtr, png_alloc_size_t size)
	{
		void *result = core::Mem::allocate(size);
		return result;
	}
	
	void pngFree(png_structp pngPtr, void *mem)
	{
		core::Mem::deallocate(mem);
	}

	ContentManager::ContentManager() : _image(0), _isOpened(false)
	{
		
	}

	void ContentManager::cleanup()
	{
		_files.clear();
	}
	unsigned int ContentManager::registerTexture(const char *name)
	{
		unsigned int result = _files.size();
		_files.push_back(name);
		return result;
	}

#define PNGSIGSIZE 8

	png_bytep * m_RowPtrs = 0;


	graphics::Image2dData * ContentManager::loadTexture(int id)
	{


		const char *name = _files[id].c_str();
		//todo: load data from content provider
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
		for (size_t i = 0; i < (size_t)m_Height; i++)
			m_RowPtrs[i] = (png_byte*)_image->Pixels + i*m_Width*m_Channels;
		
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
		return _image;
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

	int API_CALL ContentManager::registerImage(char * name) 
	{
		return registerTexture(name);
	}
	
	void API_CALL ContentManager::startLoad()
	{
		open();
	}
	
	void API_CALL ContentManager::loadImage(int id, llge::ITextureImage2d *textureImage)
	{
		graphics::Image2dData * image = loadTexture(id);
		textureImage->LoadPixels(image->Width, image->Height, (llge::TextureImage2dFormat)image->Format, image->Pixels);
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

	
	void API_CALL ContentManager::finishLoad()
	{
		close();
	}
	void API_CALL ContentManager::dispose()
	{
		delete this;
	}
}