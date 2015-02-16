// llge.cpp : Defines the exported functions for the DLL application.
//

#include "llge.h"
#include "..\core\core.h"
#include "..\graphics\graphics.h"

#include <limits>

using namespace graphics;
using namespace core;

namespace llge
{	
	class Texture : public ITexture
	{
	public:
		TextureImage2d *image;
		
		Texture()
		{
			image = new TextureImage2d();
		}

		~Texture()
		{
			delete image;
		}
		
		virtual int API_CALL getId()
		{
			return image->getHandle();
		}
		virtual void API_CALL LoadPixels(int width, int height, void *pixels)
		{
			Image2dData data(width, height);
			data.Pixels = (unsigned int *)pixels;
			image->setData(&data);
			data.Pixels = 0;
		}

		virtual void API_CALL create()
		{
			image->create();
		}
		virtual void API_CALL cleanup()
		{
			image->cleanup();
		}
		virtual void API_CALL dispose()
		{
			delete this;
		}

	};
	
	class UniformsFacade : public IUniformsFacade
	{
	public:
		MatrixContainer Projection;
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
		EffectBase * effects[32];
		VertexFormat * formats[32];

		GraphicsDevice *graphicsDevice;
		UniformsFacade *uniformsFacade;

		GraphicsFacade()
		{
			graphicsDevice = new GraphicsDevice();
			uniformsFacade = new UniformsFacade();

			effects[EffectTextureColor] = Effects::textureColor();
			effects[EffectTextureLightmapColor] = Effects::textureLightmapColor();

			formats[FormatPositionTextureColor] = VertexFormats::positionTextureColor();
		}

		~GraphicsFacade()
		{
			delete graphicsDevice;
			delete uniformsFacade;
		}

		virtual IUniformsFacade * API_CALL getUniforms()
		{
			return uniformsFacade;
		}

		virtual ITexture * API_CALL createTexture()
		{
			return new Texture();
		}

		virtual IVertexBuffer * API_CALL createVertexBuffer()
		{
			return new VBuffer();
		}
		
		virtual void API_CALL viewport(int width, int height)
		{
			graphicsDevice->setViewport(0, 0, width, height);
		}

		virtual void API_CALL setClearState(uint color, float depth)
		{
			graphicsDevice->setClearState(color, depth);
		}

		virtual void API_CALL clear()
		{
			graphicsDevice->clear();
		}

		virtual void API_CALL draw(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, void *indices, int primitivesCount)
		{
			graphicsDevice->renderState.setEffect(effects[effect]);
			graphicsDevice->drawPrimitives(formats[vertexFormat], vertices, (unsigned short *)indices, primitivesCount);
		}

		virtual void API_CALL drawVertexBuffer(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer *vertexBuffer, void *indices, int primitivesCount)
		{
			graphicsDevice->renderState.setEffect(effects[effect]);
			graphicsDevice->drawVertexBuffer(formats[vertexFormat], vertexBuffer->getId(), (unsigned short *)indices, primitivesCount);
		}

		virtual void API_CALL create()
		{
			graphicsDevice->renderState = RenderState(); /// ???
			graphicsDevice->renderState.init();
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
#ifdef __ANDROID__
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
			return new GraphicsFacade();
		}

		virtual void API_CALL dispose() 
		{
			delete this;
		}
	};

	extern "C" DLLEXPORT  IGraphicsFactory * API_CALL createGraphicsFactory()
	{
		return new GraphicsFactory();
	}
}