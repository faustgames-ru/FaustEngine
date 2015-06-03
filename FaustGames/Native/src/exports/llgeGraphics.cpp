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
		RenderConverter _converter;

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
			Effects::create();
			VertexFormats::create();
		}

		virtual void API_CALL cleanup()
		{
			Effects::cleanup();
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

		virtual ITextureImage2d * API_CALL createTextureImage2d(bool generateMipmaps)
		{
			return new TextureImage2d(generateMipmaps);
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
			return HollowsAllocationBlock::AllocationBlocksSize;
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