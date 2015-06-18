#include "TextureRenderTarget2d.h"
#include "TextureImage2d.h"
#include "Errors.h"

namespace graphics
{
    TextureRenderTarget2d::TextureRenderTarget2d() : _filter(true)
    {
        _handle = 0;
        _handleDefault = TextureImage2d::_empty.getHandle();
		X = 0;
		Y = 0;
		W = 1;
		H = 1;
	}
	
	void API_CALL TextureRenderTarget2d::create(int width, int height)
	{
		_width = width;
		_height = height;
		
		glGenTextures(1, &_handle);
		Errors::check(Errors::GenTextures);
		glActiveTexture(GL_TEXTURE0);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter ? GL_LINEAR : GL_NEAREST);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter ? GL_LINEAR : GL_NEAREST);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		Errors::check(Errors::TexParameteri);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		Errors::check(Errors::TexImage2D);


		glGenFramebuffers(1, &_frameBuffer);
		Errors::check(Errors::GenFramebuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		Errors::check(Errors::BindFramebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _handle, 0);
		Errors::check(Errors::FramebufferTexture2D);

		glGenRenderbuffers(1, &_depthBuffer);
		Errors::check(Errors::GenRenderbuffers);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		Errors::check(Errors::BindRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		Errors::check(Errors::RenderbufferStorage);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
		Errors::check(Errors::FramebufferRenderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Errors::check(Errors::BindFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		Errors::check(Errors::BindRenderbuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}
	
	void API_CALL TextureRenderTarget2d::cleanup()
	{
		glDeleteTextures(1, &_handle);
		Errors::check(Errors::DeleteTexture);
		glDeleteFramebuffers(1, &_frameBuffer);
		Errors::check(Errors::DeleteFramebuffers);
		glDeleteRenderbuffers(1, &_depthBuffer);
		Errors::check(Errors::DeleteRenderbuffers);
		_frameBuffer = 0;
		_depthBuffer = 0;
	}
}