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

	int GraphicsConfig::getMagFilter()
	{
		switch (filterMode)
		{
		case FilterMode::Nearest:
			return GL_NEAREST;
		case FilterMode::Linear:
			return GL_LINEAR;
		case FilterMode::NearestMipmapNearest:
			return GL_NEAREST;
		case FilterMode::LinearMipmapNearest:
			return GL_LINEAR;
		case FilterMode::NearestMipmapLinear:
			return GL_NEAREST;
		case FilterMode::LinearMipmapLinear:
			return GL_NEAREST;
		default:
			return GL_LINEAR;
		}
	}

	int GraphicsConfig::getMinFilter()
	{
		switch (filterMode)
		{
		case FilterMode::Nearest:
			return GL_NEAREST;
		case FilterMode::Linear:
			return GL_LINEAR;
		case FilterMode::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case FilterMode::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case FilterMode::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_NEAREST;
		case FilterMode::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			return GL_LINEAR;
		}
	}

	std::string Extensions::names[Count] = 
	{
		"OES_depth_texture"
	};

	bool GraphicsDevice::extensions[Extensions::Count] =
	{
		false
	};



	GraphicsConfig::GraphicsConfig() :
		filterMode(FilterMode::Linear),
		generateMipmaps(false),
		earlyDepthPath(true),
		enableFog(false),
		earlyFragmentTestsShaderCode(false),
		bloomDownsample(2),
		mipmapsLevel(0),
		refraction(false),
		refractionScale(1.0f),
		refractionVelocityX(1.0f),
		refractionVelocityY(1.0f),
		vignetting(false),
		vignettingR0(0.0f),
		vignettingR1(0.0f),
		vignettingR2(0.0f),
		vignettingColor0(0x0),
		vignettingColor1(0x0),
		vignettingColor2(0x0),
		ellapsedTime(0.0f),
		postEffectsScale(1.0f),
		useRgbTransforms(false)
	{

	}

	GraphicsDevice::GraphicsDevice() : actualRenderTarget(0), _depthState(-1.0f), _colorState(0), _activeTextureState(-1), _drawCalls(0)
	{
		PostProcessTargets.filter = false;
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
#ifdef __ANDROID__
            glClearDepthf(depth);
#elif __APPLE__
        
#if TARGET_OS_IPHONE
 glClearDepthf(depth);
#elif TARGET_OS_MAC
glClearDepth(static_cast<double>(depth));
#endif
#else
            glClearDepth(static_cast<double>(depth));
#endif
            
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


		config.postEffectsScale = core::Math::clamp(config.postEffectsScale, 0.25f, 1.0f);
		int w = width*config.postEffectsScale;
		int h = height*config.postEffectsScale;
		
		//PostProcessDepthTargets.setViewport(w, h);
		PostProcessTargets.setViewport(w, h);
		if (config.bloomDownsample <= 0)
			config.bloomDownsample = 2;
		//PostProcessScaledTargets.setViewport(width / config.bloomDownsample, height / config.bloomDownsample);
		PostProcessScaledTargets1.setViewport(w / 2, h / 2);
		PostProcessScaledTargets2.setViewport(w / 4, h / 4);
		PostProcessScaledTargets3.setViewport(w / 8, h / 8);

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
			

			glViewport(0, 0, renderTarget->getWidth(), renderTarget->getHeight());
			Errors::check(Errors::Viewport);
		}
	}

	void GraphicsDevice::clear()
	{
		renderState.setDepth(DepthState::ReadWrite);
		renderState.applyDepth();
		renderState.setBlend(BlendState::Alpha);
		renderState.applyBlend();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Errors::check(Errors::Clear);
	}

	void GraphicsDevice::clearDepth()
	{
		renderState.setDepth(DepthState::ReadWrite);
		renderState.applyDepth();
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


	void GraphicsDevice::readPixels(void* pixels, bool inverse)
	{
		//glGetTexImage(actualRenderTarget->getTextureHandle(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glReadPixels(0, 0, getPixelsWidth(), getPixelsHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		if (inverse)
		{
			unsigned char* chras = static_cast<unsigned char*>(pixels);
			int size = getPixelsWidth()*getPixelsHeight();
			unsigned char c0, c2;
			for (int i = 0; i < size; i += 4)
			{
				c0 = chras[0];
				c2 = chras[2];
				chras[0] = c2;
				chras[2] = c0;
			}
		}
		Errors::check(Errors::UnknownAction);
	}
	
	void GraphicsDevice::create()
	{        
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
   //TODO: FIX FOR IPHONE!!!!
//        const char* oesExts = static_cast<const char*>(static_cast<const void *>(glGetString(GL_EXTENSIONS)));
//        for (int i = 0; i < Extensions::Count; i++)
//        {
//            extensions[i] = strstr(oesExts, Extensions::names[i].c_str()) != nullptr;
//        }
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
		IPostProcessTarget * rt = _constructor->createPostProcessTarget(filter);
		rt->create(_width, _height);
		_stack.push_back(rt);
		_all.push_back(rt);
	}


	PostProcessTarget::PostProcessTarget(TextureRenderTarget2d* target): _target(target)
	{
	}

	IRenderTarget* PostProcessTarget::getRenderTarget()
	{
		return _target;
	}

	Texture* PostProcessTarget::getColor()
	{
		return _target;
	}

	Texture* PostProcessTarget::getDepth()
	{
		return nullptr;
	}

	int PostProcessTarget::getWidth()
	{
		return _target->getWidth();
	}

	int PostProcessTarget::getHeight()
	{
		return _target->getHeight();
	}

	void PostProcessTarget::create(int width, int height)
	{
		_target->create(width, height);
	}

	void PostProcessTarget::cleanup()
	{
		_target->cleanup();
	}

	PostProcessDepthTarget::PostProcessDepthTarget(TextureRenderTargetDepth2d* target)
	{
		_target = target;
	}

	IRenderTarget* PostProcessDepthTarget::getRenderTarget()
	{
		return _target;
	}

	Texture* PostProcessDepthTarget::getColor()
	{
		return &_target->_colorTexture;
	}

	Texture* PostProcessDepthTarget::getDepth()
	{
		return &_target->_depthTexture;
	}

	int PostProcessDepthTarget::getWidth()
	{
		return _target->getWidth();
	}

	int PostProcessDepthTarget::getHeight()
	{
		return _target->getHeight();
	}

	void PostProcessDepthTarget::create(int width, int height)
	{
		_target->create(width, height);
	}

	void PostProcessDepthTarget::cleanup()
	{
		_target->cleanup();
	}

	PostProcessTargetConstructor PostProcessTargetConstructor::Default;


	IPostProcessTarget* PostProcessTargetConstructor::createPostProcessTarget(bool filter)
	{
		return new PostProcessTarget(new TextureRenderTarget2d(filter));
	}

	PostProcessTargetDepthConstructor PostProcessTargetDepthConstructor::Default;


	IPostProcessTarget* PostProcessTargetDepthConstructor::createPostProcessTarget(bool filter)
	{
		return new PostProcessDepthTarget(new TextureRenderTargetDepth2d());
	}

	PostProcessTargetManager::PostProcessTargetManager()
	{
		_constructor = &PostProcessTargetConstructor::Default;
		filter = true;
		_width = 0;
		_height = 0;
	}

	void PostProcessTargetManager::setConstructor(IPostProcessTargetConstructor* constructor)
	{
		_constructor = constructor;
	}

	IPostProcessTarget *PostProcessTargetManager::pop()
	{
		if (_stack.size() == 0)
		{
			addProcessRenderTarget();
		}
		IPostProcessTarget * res = _stack[_stack.size() - 1];
		_stack.pop_back();
		return res;
	}
	
	void PostProcessTargetManager::push(IPostProcessTarget *rt)
	{
		_stack.push_back(rt);
	}
	
	void PostProcessTargetManager::setViewport(int width, int height)
	{
		_width = width;
		_height = height;
		for (uint i = 0; i < _all.size(); i++)
		{
			IPostProcessTarget *rt = _all[i];
			if ((rt->getWidth() != width) || (rt->getHeight() != height))
			{
				rt->cleanup();
				rt->create(width, height);
			}
		}
	}

	bool PostProcessTargetManager::isAvaliable()
	{
		return _width > 0 && _height > 0;
	}
}
