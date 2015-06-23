#include "TextureImage2d.h"
#include "Uniforms.h"
#include "Errors.h"

namespace graphics
{

	struct ImageColor
	{
		unsigned int value;
		ImageColor(unsigned int v)
		{
			value = v;
		}
		unsigned int getComponent(int i)
		{
			return (value >> (i * 8)) & 0xff;
		}
		void  setComponent(int i, int v)
		{
			value &= 0xff << (i * 8);
			value |= v << (i * 8);
		}
	};

	TextureImage2d::TextureImage2d() : _wrap(false), _filter(true), _createMipmaps(false)
	{
		_size = 0;
		_handle = 0;
		X = 0;
		Y = 0;
		W = 1;
		H = 1;
		_handleDefault = _empty.getHandle();
	}
	
	TextureImage2d::TextureImage2d(bool generateMipmaps, bool useFilter) : _wrap(false), _filter(useFilter), _createMipmaps(generateMipmaps)
	{
		_size = 0;
		_handle = 0;
		_handleDefault = _empty.getHandle();
		X = 0;
		Y = 0;
		W = 1;
		H = 1;
	}
	
	void API_CALL TextureImage2d::LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels)
	{
		setData(width, height, (Image2dFormat::e)format, (unsigned int *)pixels);
	}
	
	void API_CALL TextureImage2d::create()
	{
		//UniformValues::resetSamplers();
		glGenTextures(1, &_handle);
		Errors::check(Errors::GenTextures);
		glActiveTexture(GL_TEXTURE0 + GraphicsConstants::Samplers2DStart);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);
		if (_filter)
		{
			if (_createMipmaps)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Errors::check(Errors::TexParameteri);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				Errors::check(Errors::TexParameteri);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Errors::check(Errors::TexParameteri);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				Errors::check(Errors::TexParameteri);
			}
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			Errors::check(Errors::TexParameteri);
		}

		if (_wrap) 
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
		}
		else 
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}

	void API_CALL TextureImage2d::cleanup()
	{
		Size -= _size;
		_size = 0;
		glDeleteTextures(1, &_handle);
		Errors::check(Errors::DeleteTexture);
	}

	void TextureImage2d::setData(int width, int height, Image2dFormat::e format, unsigned int *pixels)
	{
		Size -= _size;
		_size = width * height;
		switch (format)
		{
		case Image2dFormat::Rgb:
			_size *= 3;
			break;
		case Image2dFormat::Rgba:
			_size *= 4;
			break;
		default:
			break;
		}
		Size += _size;
		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);
		if (_createMipmaps)
		{
			int w = width;
			int h = height;

			int mip = 0;
			while ((w > 0) || (h > 0))
			{
				glTexImage2D(GL_TEXTURE_2D, mip, getFormat(format), w == 0 ? 1 : w, h == 0 ? 1 : h, 0, getFormat(format), GL_UNSIGNED_BYTE, pixels);
				Errors::check(Errors::TexImage2D);
				w /= 2;
				h /= 2;
				int i = 0;
				for (int y = 0; y < h; y++)
				{
					for (int x = 0; x < w; x++)
					{
						int i0 = (y * 2 + 0) * w * 2 + x * 2 + 0;
						int i1 = (y * 2 + 1) * w * 2 + x * 2 + 0;
						int i2 = (y * 2 + 0) * w * 2 + x * 2 + 1;
						int i3 = (y * 2 + 1) * w * 2 + x * 2 + 1;
						
						ImageColor c0 = ImageColor(pixels[i0]);
						ImageColor c1 = ImageColor(pixels[i1]);
						ImageColor c2 = ImageColor(pixels[i2]);
						ImageColor c3 = ImageColor(pixels[i3]);
						ImageColor c(0);
						unsigned int r = (c0.getComponent(0) + c1.getComponent(0) + c2.getComponent(0) + c3.getComponent(0)) / 4;
						unsigned int g = (c0.getComponent(1) + c1.getComponent(1) + c2.getComponent(1) + c3.getComponent(1)) / 4;
						unsigned int b = (c0.getComponent(2) + c1.getComponent(2) + c2.getComponent(2) + c3.getComponent(2)) / 4;
						unsigned int a = (c0.getComponent(3) + c1.getComponent(3) + c2.getComponent(3) + c3.getComponent(3)) / 4;
						c.setComponent(0, r);
						c.setComponent(1, g);
						c.setComponent(2, b);
						c.setComponent(3, a);
						pixels[y * w + x] = c0.value;
						i++;
					}
				}
				mip++;
			}
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, getFormat(format), width, height, 0, getFormat(format), GL_UNSIGNED_BYTE, pixels);
			Errors::check(Errors::TexImage2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}


	void TextureImage2d::setData(const Image2dData *data)
	{
		setData(data->Width, data->Height, data->Format, data->Pixels);
	}

	GLenum TextureImage2d::getFormat(Image2dFormat::e format)
	{
		switch (format)
		{
		case Image2dFormat::Rgb:
			return GL_RGB;
		case Image2dFormat::Rgba:
			return GL_RGBA;
		default:
			return GL_RGBA;
		}
	}

	void TextureImage2d::createStatic()
	{
		_empty.create();
		unsigned int blackPixel = 0x00000000;
		_empty.setData(1, 1, Image2dFormat::Rgba, &blackPixel);
	}

	void TextureImage2d::cleanupStatic()
	{
		_empty.cleanup();
	}

	int TextureImage2d::Size(0);

	TextureImage2d TextureImage2d::_empty;
/*
	TextureImage2dProxy::TextureImage2dProxy()
	{
		_handle = 0;
		_instance = 0;
		_handleDefault = TextureImage2d::_empty.getHandle();
		_isRealyProxy = false;
		X = 0;
		Y = 0;
		W = 1;
		H = 1;
	}
	TextureImage2dProxy::~TextureImage2dProxy()
	{
		if (_isRealyProxy) return;
		if (_instance)
		{
			_instance->dispose();
		}
	}

	void TextureImage2dProxy::setProxyInstance(TextureImage2d * instance)
	{
		_isRealyProxy = true;
		_instance = instance;
		_handle = _instance->getId();
	}

	llge::ITexture* API_CALL TextureImage2dProxy::getTexture()
	{
		return this;
	}

	void API_CALL TextureImage2dProxy::LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels)
	{
		if (_isRealyProxy) return;
		if (!_instance)
        {
			_instance = new TextureImage2d(false, true);
            _instance->create();
        }
        _handle = _instance->getId();
		_instance->LoadPixels(width, height, format, pixels);
	}

	void API_CALL TextureImage2dProxy::create()	{ }

	void API_CALL TextureImage2dProxy::cleanup()
	{
		if (_isRealyProxy) return;
		if (_instance)
		{
			_instance->cleanup();
		}
	}

	void API_CALL TextureImage2dProxy::dispose()
	{
		delete this;
	}
 */
}