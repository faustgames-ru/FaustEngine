#include "TextureRenderTargetDepth2d.h"
#include "Errors.h"

namespace graphics
{
	TextureRenderTargetDepth2d::TextureRenderTargetDepth2d() : _filter(false)
	{
	}

	void API_CALL TextureRenderTargetDepth2d::create(int width, int height)
	{
		_width = width;
		_height = height;

		GLuint colorHandle;
		GLuint depthHandle;

		glGenFramebuffers(1, &_frameBuffer);
		Errors::check(Errors::GenFramebuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		Errors::check(Errors::BindFramebuffer);

		glGenTextures(1, &colorHandle);
		Errors::check(Errors::GenTextures);
		glActiveTexture(GL_TEXTURE0);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, colorHandle);
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

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorHandle, 0);
		Errors::check(Errors::FramebufferTexture2D);

		glGenTextures(1, &depthHandle);
		Errors::check(Errors::GenTextures);
		glBindTexture(GL_TEXTURE_2D, depthHandle);
		Errors::check(Errors::BindTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter ? GL_LINEAR : GL_NEAREST);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter ? GL_LINEAR : GL_NEAREST);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		Errors::check(Errors::TexParameteri);

		// todo: work around gles2 depth texture support
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		Errors::check(Errors::TexParameteri);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		Errors::check(Errors::TexParameteri);
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		Errors::check(Errors::TexImage2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthHandle, 0);
		Errors::check(Errors::FramebufferTexture2D);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Errors::check(Errors::BindFramebuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);

		_colorTexture.setHandle(colorHandle);
		_depthTexture.setHandle(depthHandle);
	}

	void API_CALL TextureRenderTargetDepth2d::cleanup()
	{
		GLuint colorHandle = _colorTexture.getHandle();
		GLuint depthHandle = _depthTexture.getHandle();

		glDeleteTextures(1, &colorHandle);
		Errors::check(Errors::DeleteTexture);
		glDeleteTextures(1, &depthHandle);
		Errors::check(Errors::DeleteTexture);
		glDeleteFramebuffers(1, &_frameBuffer);
		Errors::check(Errors::DeleteFramebuffers);
		_frameBuffer = 0;
		_colorTexture.setHandle(0);
		_colorTexture.setHandle(0);
	}
}