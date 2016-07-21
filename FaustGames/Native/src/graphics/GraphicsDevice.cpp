#include "GraphicsDevice.h"
#include "Color.h"
#include "Errors.h"
#include "UniformValueTexture.h"
#include "Uniforms.h"
#include "TextureImage2d.h"

namespace graphics
{
	GraphicsDevice GraphicsDevice::Default;
    int GraphicsDevice::_primaryFbo(0);

	GraphicsDevice::GraphicsDevice() : _colorState(0), _depthState(-1.0f), _activeTextureState(-1), _drawCalls(0), actualRenderTarget(0)
	{
	}
	
	GraphicsDevice::~GraphicsDevice()
	{
	}


	void GraphicsDevice::setClearState(unsigned int color, float depth)
	{
        if (color != _colorState)
        {
            glClearColor((float)Color::getR(color) / 255.0f, (float)Color::getG(color) / 255.0f, (float)Color::getB(color) / 255.0f, (float)Color::getA(color) / 255.0f);
			_colorState = color;
        }
        if (core::Math::abs(depth - _depthState) > 0.0001f)
        {
            glClearDepthf(depth);
			_depthState = depth;
        }
	}


	void GraphicsDevice::setViewport(int x, int y, int width, int height)
	{
        if ((_viewportX != x) || (_viewportY != y) || (_viewportWidth != width) || (_viewportHeight != height))
        {
            _viewportX = x;
            _viewportY = y;
            _viewportWidth = width;
            _viewportHeight = height;
            glViewport(x, y, width, height);
            Errors::check(Errors::Viewport);
        }
		PostProcessTargets.setViewport(width, height);
		PostProcessScaledTargets.setViewport(width / 8, height / 8);
		/*
		float rtWidth = width / 2;
		float rtHeight = height / 2;
		for (int i = 0; i < PostProcessRenderTargets.size(); i++)
		{
			TextureRenderTarget2d *rt = PostProcessRenderTargets[i];
			if (i > 1)
			{
				if ((rt->getWidth() != rtWidth) || (rt->getHeight() != rtHeight))
				{
					rt->cleanup();
					rt->create(rtWidth, rtHeight);
				}
			}
			else
			{
				if ((rt->getWidth() != width) || (rt->getHeight() != height))
				{
					rt->cleanup();
					rt->create(width, height);
				}
			}
		}
		*/
	}
	
	void GraphicsDevice::setRenderTarget(IRenderTarget *renderTarget)
	{
		actualRenderTarget = renderTarget;
		if (!renderTarget)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _primaryFbo);
			Errors::check(Errors::BindFramebuffer);
			glViewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
			Errors::check(Errors::Viewport);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->getFramebuffer());
			Errors::check(Errors::BindFramebuffer);

			/*
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			Errors::check(Errors::CheckFramebufferStatus);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				switch (status)
				{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					Errors::rise(Errors::CheckFramebufferStatus);
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					Errors::rise(Errors::CheckFramebufferStatus);
					break;
				
				//case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				//	Errors::check(Errors::CheckFramebufferStatus);
				//	break;
				//case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				//	Errors::check(Errors::CheckFramebufferStatus);
				//	break;
				case GL_FRAMEBUFFER_UNSUPPORTED:
					Errors::rise(Errors::CheckFramebufferStatus);
					break;
				}
				return;
			}
			*/

			glViewport(0, 0, renderTarget->getWidth(), renderTarget->getHeight());
			Errors::check(Errors::Viewport);
		}
	}

	void GraphicsDevice::clear()
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Errors::check(Errors::Clear);
	}

	void GraphicsDevice::clearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		Errors::check(Errors::Clear);
	}

	void GraphicsDevice::resetState()
	{
		renderState = RenderState();
		_viewportX = 0;
		_viewportY = 0;
		_viewportWidth = 0;
		_viewportHeight = 0;
		_activeTextureState = -1;
	}


	void GraphicsDevice::drawVertexBuffer(VertexFormat *vertexFormat, GLuint vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(vertexBuffer);
		renderState.apply(vertexFormat, 0);
		glDrawElements(GL_TRIANGLES, primitivesCount * 3, GL_UNSIGNED_SHORT, indexBuffer);
		Errors::check(Errors::DrawElements);
		++_drawCalls;
	}

	void GraphicsDevice::drawEdgesPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(0);
		renderState.apply(vertexFormat, vertexBuffer);
		glDrawElements(GL_LINES, primitivesCount * 2, GL_UNSIGNED_SHORT, indexBuffer);
		Errors::check(Errors::DrawElements);
		++_drawCalls;
	}

	void GraphicsDevice::drawEdges(VertexFormat *vertexFormat, void *vertexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(0);
		renderState.apply(vertexFormat, vertexBuffer);
		glDrawArrays(GL_LINES, 0, primitivesCount * 2);
		Errors::check(Errors::DrawElements);
		++_drawCalls;
	}
	
	void GraphicsDevice::drawTriangles(VertexFormat *vertexFormat, void *vertexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(0);
		renderState.apply(vertexFormat, vertexBuffer);
		glDrawArrays(GL_TRIANGLES, 0, primitivesCount * 3);
		Errors::check(Errors::DrawElements);
		++_drawCalls;
	}

	void GraphicsDevice::drawPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
		renderState.setVertexBuffer(0);
		renderState.apply(vertexFormat, vertexBuffer);
		glDrawElements(GL_TRIANGLES, primitivesCount * 3, GL_UNSIGNED_SHORT, indexBuffer);
		Errors::check(Errors::DrawElements);
		++_drawCalls;
	}

	void GraphicsDevice::resetSamplersState()
	{
		_activeTextureState = -1;
	}

	void GraphicsDevice::resetRenderState()
	{
		renderState = RenderState();
	}

    void GraphicsDevice::grabDefaultRenderTarget()
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_primaryFbo);
    }
    
    void GraphicsDevice::setActiveTexture(uint value)
    {
        if (value != _activeTextureState)
        {
            glActiveTexture(GL_TEXTURE0 + GraphicsConstants::Samplers2DStart + value);
            Errors::check(Errors::ActiveTexture);
            _activeTextureState = value;
        }
    }

	int GraphicsDevice::getPixelsWidth() 
	{
		if (actualRenderTarget)
		{
			return actualRenderTarget->getWidth();
		}
		else
		{
			return _viewportWidth;
		}
	}
	
	int GraphicsDevice::getPixelsHeight() 
	{
		if (actualRenderTarget)
		{
			return actualRenderTarget->getHeight();
		}
		else
		{
			return _viewportHeight;
		}
	}


	void GraphicsDevice::readPixels(void* pixels)
	{
		glReadPixels(0, 0, getPixelsWidth(), getPixelsHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
	
	void GraphicsDevice::create()
	{        
		TextureImage2d::createStatic();
	}
	
	void GraphicsDevice::resetDrawCallsCounter()
	{
		_drawCalls = 0;
	}
	
	int GraphicsDevice::getDrawCallsCounterValue()
	{
		return _drawCalls;
	}
	
	void PostProcessTargetManager::addProcessRenderTarget()
	{
		TextureRenderTarget2d * rt = new TextureRenderTarget2d();
		rt->create(128, 128);
		_stack.push_back(rt);
		_all.push_back(rt);
	}


	TextureRenderTarget2d *PostProcessTargetManager::pop()
	{
		if (_stack.size() == 0)
		{
			addProcessRenderTarget();
		}
		TextureRenderTarget2d * res = _stack[_stack.size() - 1];
		_stack.pop_back();
		return res;
	}
	
	void PostProcessTargetManager::push(TextureRenderTarget2d *rt)
	{
		_stack.push_back(rt);
	}
	
	void PostProcessTargetManager::setViewport(int width, int height)
	{
		for (uint i = 0; i < _all.size(); i++)
		{
			TextureRenderTarget2d *rt = _all[i];
			if ((rt->getWidth() != width) || (rt->getHeight() != height))
			{
				rt->cleanup();
				rt->create(width, height);
			}
		}
	}

}
