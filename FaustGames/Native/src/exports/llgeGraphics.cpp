// llge.cpp : Defines the exported functions for the DLL application.
//

#include "llge.h"
#include "../core/core.h"
#include "../graphics/graphics.h"

#include <limits>

using namespace graphics;
using namespace core;

namespace llge
{				
	class UniformsFacade : public IUniformsFacade
	{
	public:
		MatrixContainer Projection;

		virtual void API_CALL resetSamplers()
		{
			GraphicsDevice::Default.resetRenderState();
			UniformValues::resetSamplers();
		}
		
		virtual void API_CALL setTime(float value)
		{
			UniformValues::time()->setValue(value);
		}

		virtual void API_CALL setNormalmap(ITexture *texture)
		{
			UniformValues::normalmap()->setValue(texture->getId());
		}

		virtual void API_CALL setEnvironment(ITexture *cubemap)
		{
			UniformValues::environment()->setValue(cubemap->getId());
		}

		virtual void API_CALL setDepthmap(ITexture *texture)
		{
			UniformValues::depthmap()->setValue(texture->getId());
		}

		
		virtual void API_CALL setTexture(ITexture *texture)
		{
			UniformValues::texture()->setValue(texture->getId());
		}
		
		virtual void API_CALL setLightMap(ITexture *texture)
		{
			UniformValues::lightmap()->setValue(texture->getId());
		}
		
		virtual void API_CALL setProjection(void *floatMatrix) 
		{
			Projection.setValue((float *)floatMatrix);
			UniformValues::projection()->setValue(Projection);
		}

		virtual void API_CALL setFogStart(float value)
		{
			UniformValues::fogStart()->setValue(value);
		}
		
		virtual void API_CALL setFogDensity(float value)
		{
			UniformValues::fogDensity()->setValue(value);
		}
		
		virtual void API_CALL setFogScale(float value)
		{
			UniformValues::fogScale()->setValue(value);
		}

		virtual void API_CALL setFogColor(float r, float g, float b)
		{
			UniformValues::fogColor()->setValue(core::Vector3(r, g, b));
		}

	};

	class VBuffer : public IVertexBuffer
	{
	public:
		graphics::VertexBuffer vertexBuffer;
		virtual int API_CALL getId()
		{
			return vertexBuffer.getHandle(); 
		}
		
		virtual void API_CALL create()
		{
			vertexBuffer.create();
		}
		
		virtual void API_CALL setData(void *data, int count)
		{
			vertexBuffer.setData(data, count);
		}
		
		virtual void API_CALL cleanup()
		{
			vertexBuffer.cleanup();
		}
		
		virtual void API_CALL dispose()
		{
			delete this;
		}
	};

	class GraphicsFacade : public IGraphicsFacade
	{
	public:
		RENDER_CONVERTER_CLASS _converter;

		GraphicsDevice *graphicsDevice;
		UniformsFacade *uniformsFacade;

		graphics::BlendState::e _blendMode;

		GraphicsFacade()
		{
			graphicsDevice = &GraphicsDevice::Default;
			uniformsFacade = new UniformsFacade();
		}

		~GraphicsFacade()
		{
			delete uniformsFacade;
		}

		virtual void API_CALL resetDrawCallsCounter()
		{
			graphicsDevice->resetDrawCallsCounter();
		}
		
		virtual int API_CALL getDrawCallsCounterValue()
		{
			return graphicsDevice->getDrawCallsCounterValue();
		}


		virtual void API_CALL setEffectConstantFloat(GraphicsEffects effect, char *name, float value)
		{
			EffectConstant* constant = _converter.getEffect(effect)->getEffect()->findConstant(name);
			constant->setFloat(value);
		}

		virtual void API_CALL setEffectConstantColor(GraphicsEffects effect, char *name, uint value)
		{
			EffectConstant* constant = _converter.getEffect(effect)->getEffect()->findConstant(name);
			constant->setUint(value);
		}

		virtual IUniformsFacade * API_CALL getUniforms()
		{
			return uniformsFacade;
		}

		virtual void API_CALL setRenderTarget(void *renderTargetInstance)
		{
			graphicsDevice->setRenderTarget((IRenderTarget *)renderTargetInstance);
		}

		virtual void API_CALL viewport(int width, int height)
		{
			graphicsDevice->setViewport(0, 0, width, height);
		}

		virtual void API_CALL setBlendMode(BlendMode blendMode)
		{
			_blendMode = (graphics::BlendState::e)blendMode;
		}

		virtual void API_CALL setClearState(uint color, float depth)
		{
			graphicsDevice->setClearState(color, depth);
		}

		virtual void API_CALL clear()
		{
			graphicsDevice->clear();
		}

		virtual void API_CALL clearDepth()
		{
			graphicsDevice->clearDepth();
		}


		virtual void API_CALL drawEdges(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, int primitivesCount)
		{
			graphicsDevice->renderState.setBlend(_blendMode);
			graphicsDevice->renderState.setEffect(_converter.getEffect(effect));
			graphicsDevice->drawEdges(_converter.getFormat(vertexFormat), vertices, primitivesCount);
		}
		
		virtual void API_CALL draw(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, int primitivesCount)
		{
			graphicsDevice->renderState.setBlend(_blendMode);
			graphicsDevice->renderState.setEffect(_converter.getEffect(effect));
			graphicsDevice->drawTriangles(_converter.getFormat(vertexFormat), vertices, primitivesCount);
		}
		
		virtual void API_CALL drawElements(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, void *indices, int primitivesCount)
		{
			graphicsDevice->renderState.setBlend(_blendMode);
			graphicsDevice->renderState.setEffect(_converter.getEffect(effect));
			graphicsDevice->drawPrimitives(_converter.getFormat(vertexFormat), vertices, (unsigned short *)indices, primitivesCount);
		}

		virtual void API_CALL drawVertexBuffer(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer *vertexBuffer, void *indices, int primitivesCount)
		{
			graphicsDevice->renderState.setBlend(_blendMode);
			graphicsDevice->renderState.setEffect(_converter.getEffect(effect));
			graphicsDevice->drawVertexBuffer(_converter.getFormat(vertexFormat), vertexBuffer->getId(), (unsigned short *)indices, primitivesCount);
		}

        virtual void API_CALL grabDefaultRenderTarget()
        {
            graphicsDevice->grabDefaultRenderTarget();
        }
        
		virtual void API_CALL create()
		{
			graphicsDevice->renderState = RenderState(); /// ???
			graphicsDevice->renderState.init();
			GraphicsDevice::create();
			EFFECTS_CALL_CREATE
			FORMATS_CALL_CREATE
		}

		virtual int API_CALL getPixelsWidth()
		{
			return graphicsDevice->getPixelsWidth();
		}

		virtual int API_CALL getPixelsHeight() 
		{
			return graphicsDevice->getPixelsHeight();
		}

		virtual void API_CALL getPixels(IntPtr target) 
		{
			return graphicsDevice->readPixels(target);
		}


		virtual void API_CALL cleanup()
		{
			EFFECTS_CALL_CLEANUP;
		}

		virtual void API_CALL dispose()
		{
			delete this;
		}

	};

	class GraphicsFactory : public IGraphicsFactory
	{
	public:
		virtual IGraphicsFacade * API_CALL createGraphicsFacade()
		{
			return new GraphicsFacade();
		}

		virtual ITextureImage2d * API_CALL createTextureImage2d(bool generateMipmaps, bool useFilter)
		{
			return new TextureImage2d(generateMipmaps, useFilter);
		}

		virtual IRenderTarget2d * API_CALL createRenderTarget2d()
		{
			return new TextureRenderTarget2d();
		}

		virtual IRenderTargetDepth2d * API_CALL createRenderTargetDepth2d()
		{
			return new TextureRenderTargetDepth2d();
		}

		virtual IVertexBuffer * API_CALL createVertexBuffer()
		{
			return new VBuffer();
		}

		virtual void API_CALL setupGraphics(IntPtr graphicsConfig)
		{
			GraphicsConfig* config = static_cast<GraphicsConfig*>(graphicsConfig);
			GraphicsDevice::Default.config.earlyDepthPath = config->earlyDepthPath;
			GraphicsDevice::Default.config.generateMipmaps = config->generateMipmaps;

			TextureFilterMode filterMode = static_cast<TextureFilterMode>(config->texturesFilter);
			switch (filterMode)
			{
			case Nearest: 
				GraphicsDevice::Default.config.filterMode = FilterMode::Nearest;
				break;
			case Linear: 
				GraphicsDevice::Default.config.filterMode = FilterMode::Linear;
				break;
			case NearestMipmapNearest: 
				GraphicsDevice::Default.config.filterMode = FilterMode::NearestMipmapNearest;
				break;
			case LinearMipmapNearest: 
				GraphicsDevice::Default.config.filterMode = FilterMode::LinearMipmapNearest;
				break;
			case NearestMipmapLinear: 
				GraphicsDevice::Default.config.filterMode = FilterMode::NearestMipmapLinear;
				break;
			case LinearMipmapLinear: 
				GraphicsDevice::Default.config.filterMode = FilterMode::LinearMipmapLinear;
				break;
			default: 
				break;
			}
		}

		virtual void API_CALL dispose() 
		{
			delete this;
		}
	};

	class NativeMemoryProfiler : public INativeMemoryProfiler
	{
	public:
		virtual int API_CALL getTexturesSize()
		{
			return graphics::TextureImage2d::Size;
		}

		virtual int API_CALL getAllocationsSize()
		{
			return Mem::AllocatedSize;
		}

		virtual int API_CALL getHeapSize()
		{
            return 0;//HollowsAllocationBlock::AllocationBlocksSize;
		}
	};

	extern "C" DLLEXPORT  IGraphicsFactory * API_CALL createGraphicsFactory()
	{
		return new GraphicsFactory();
	}

	extern "C" DLLEXPORT void API_CALL initRenderContext()
	{
#ifdef __ANDROID__
#else
#ifdef __APPLE__
#else
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			switch (err)
			{
			case GLEW_ERROR_NO_GL_VERSION:
				break;  /* missing GL version */
			case GLEW_ERROR_GL_VERSION_10_ONLY:
				break;  /* Need at least OpenGL 1.1 */
			case GLEW_ERROR_GLX_VERSION_11_ONLY:
				break;  /* Need at least GLX 1.2 */
			default:
				break;
			}
			//throw std::exception("fail to init glew");
		}
#endif
#endif
	}

	extern "C" DLLEXPORT INativeMemoryProfiler * API_CALL createNativeMemoryProfiler() 
	{
		return new NativeMemoryProfiler();
	}

}