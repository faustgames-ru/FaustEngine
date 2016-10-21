#include "TextureCubemap.h"
#include "Errors.h"
#include "GraphicsDevice.h"

namespace graphics
{
	TextureCubemap::TextureCubemap() : _wrap(false), _filter(true)
	{
		X = 0;
		Y = 0;
		W = 1;
		H = 1;
	}
	void TextureCubemap::create()
	{
		glGenTextures(1, &_handle);
		Errors::check(Errors::GenTextures);
		
		GraphicsDevice::Default.resetSamplersState();
		glActiveTexture(GL_TEXTURE0 + GraphicsConstants::SamplersCubeStart);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
		Errors::check(Errors::BindTexture);
		if (_filter)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			Errors::check(Errors::TexParameteri);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			Errors::check(Errors::TexParameteri);
		}

		if (_wrap) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
		}
		else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		Errors::check(Errors::BindTexture);
	}

	void TextureCubemap::cleanup()
	{
		glDeleteTextures(1, &_handle);
		Errors::check(Errors::DeleteTexture);
	}

	void TextureCubemap::setData(TextureCubemapPlanes::e plane, const Image2dData *data)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
		Errors::check(Errors::BindTexture);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + plane, 0, GL_RGBA, data->Width, data->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->Pixels);
		Errors::check(Errors::TexImage2D);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		Errors::check(Errors::BindTexture);
	}

}