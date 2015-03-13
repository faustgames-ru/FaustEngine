#include "ContentManager.h"
#include "ContentProvider.h"
#include "lpng\png.h"

namespace resources
{
	void readData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		ContentProvider::read(data, length);
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

		m_PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if (!m_PngPtr)
		{
			//throw ref new Exception(-1, "png_create_read_struct failed");
			return 0;
		}
		m_InfoPtr = png_create_info_struct(m_PngPtr);

		if (!m_InfoPtr)
		{
			png_destroy_read_struct(&m_PngPtr, 0, 0);
			m_PngPtr = 0;
			//throw ref new Exception(-1, "png_create_info_struct failed");
			return 0;
		}

		png_set_read_fn(m_PngPtr, 0, readData);

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

		png_read_image(m_PngPtr, m_RowPtrs);

		png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, 0);

		ContentProvider::closeContent();
		return _image;
	}

	void ContentManager::open()
	{
		if (_isOpened) 
			return;
		m_RowPtrs = new png_bytep[ImageMaxHeight];
		_image = new graphics::Image2dData(ImageBufferSize);
		for (size_t i = 0; i < (size_t)ImageMaxHeight; i++)
			m_RowPtrs[i] = (png_byte*)(_image->Pixels + ImageMaxWidth);
	}
	void ContentManager::close()
	{
		if (!_isOpened)
			return;
		delete[] m_RowPtrs;
		delete _image;
		m_RowPtrs = 0;
		_image = 0;
	}

}