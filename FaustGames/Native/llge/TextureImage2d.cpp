#include "stdafx.h"
#include "TextureImage2d.h"
#include "Errors.h"

namespace graphics
{
	TextureImage2d::TextureImage2d() : _wrap(false)
	{
		_handle = 0;
	}

	void TextureImage2d::create()
	{
		glGenTextures(1, &_handle);
		Errors::check(Errors::GenTextures);
		glActiveTexture(GL_TEXTURE0);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		Errors::check(Errors::TexParameteri);

		if (_wrap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}

	void TextureImage2d::cleanup()
	{
		glDeleteTextures(1, &_handle);
		Errors::check(Errors::DeleteTexture);
	}
	void TextureImage2d::setData(const Image2dData *data)
	{
		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->Width, data->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->Pixels);
		Errors::check(Errors::TexImage2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}
}