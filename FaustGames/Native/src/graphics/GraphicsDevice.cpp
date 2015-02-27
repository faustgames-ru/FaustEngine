#include "GraphicsDevice.h"
#include "Color.h"
#include "Errors.h"
#include "UniformValueTexture.h"

namespace graphics
{
	GraphicsDevice GraphicsDevice::Default;


	GraphicsDevice::GraphicsDevice()
	{
	}
	
	GraphicsDevice::~GraphicsDevice()
	{
	}


	void GraphicsDevice::setClearState(unsigned int color, float depth)
	{
		glClearColor(
			(float)Color::getR(color) / 255.0f,
			(float)Color::getG(color) / 255.0f,
			(float)Color::getB(color) / 255.0f,
			(float)Color::getA(color) / 255.0f);
		glClearDepthf(depth);
	}


	void GraphicsDevice::setViewport(int x, int y, int width, int height)
	{
		_viewportX = x;
		_viewportY = y;
		_viewportWidth = width;
		_viewportHeight = height;
		glViewport(x, y, width, height);
		Errors::check(Errors::Viewport);
	}

	void GraphicsDevice::setRenderTarget(TextureRenderTarget2d *renderTarget)
	{
		if (!renderTarget)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			Errors::check(Errors::BindFramebuffer);
			glViewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
			Errors::check(Errors::Viewport);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->getFramebuffer());
			Errors::check(Errors::BindFramebuffer);
			/*
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			Errors::check(Errors::CheckFramebufferStatus);
			if (status != GL_FRAMEBUFFER_COMPLETE)
				return;
			*/
			glViewport(0, 0, renderTarget->getWidth(), renderTarget->getHeight());
			Errors::check(Errors::Viewport);
		}
	}

	void GraphicsDevice::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Errors::check(Errors::Clear);
	}

	void GraphicsDevice::resetState()
	{
		renderState = RenderState();
	}


	void GraphicsDevice::drawVertexBuffer(VertexFormat *vertexFormat, GLuint vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(vertexBuffer);
		renderState.apply(vertexFormat, 0);
		glDrawElements(GL_TRIANGLES, primitivesCount * 3, GL_UNSIGNED_SHORT, indexBuffer);
		Errors::check(Errors::DrawElements);
	}

	void GraphicsDevice::drawPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(0);
		renderState.apply(vertexFormat, vertexBuffer);
		glDrawElements(GL_TRIANGLES, primitivesCount * 3, GL_UNSIGNED_SHORT, indexBuffer);
		Errors::check(Errors::DrawElements);
	}

}
