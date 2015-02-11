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

	class VertexFormatsFacade : public IVertexFormatsFacade
	{
	public:
		VertexFormat * Formats[1];
		VertexFormatsFacade()
		{
			Formats[0] = VertexFormats::positionTextureColor();
		}
		virtual int API_CALL getPositionTextureColorFormat()
		{
			return 0;
		}
	};

	class EffectsFacade : public IEffectsFacade
	{
	public:
		EffectBase * Effects[2];
		EffectsFacade()
		{
			Effects[0] = Effects::textureColor();
			Effects[1] = Effects::textureLightmapColor();
		}
		virtual int API_CALL getTextureColorEffect()
		{
			return 0;
		}
		virtual int API_CALL getTextureLightmapColorEffect()
		{
			return 1;
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

	class GraphicsFacade : public IGraphicsFacade
	{
	public:
		GraphicsDevice *graphicsDevice;
		UniformsFacade *uniformsFacade;
		VertexFormatsFacade *vertexFormatsFacade;
		EffectsFacade *effectsFacade;

		GraphicsFacade()
		{
			graphicsDevice = new GraphicsDevice();
			uniformsFacade = new UniformsFacade();
			vertexFormatsFacade = new VertexFormatsFacade();
			effectsFacade = new EffectsFacade();
		}
		
		~GraphicsFacade()
		{
			delete graphicsDevice;
			delete uniformsFacade;
			delete vertexFormatsFacade;
			delete effectsFacade;
		}	
		
		virtual IUniformsFacade * API_CALL getUniforms()
		{
			return uniformsFacade;
		}

		virtual IVertexFormatsFacade * API_CALL getVertexFormatsFacade()
		{
			return vertexFormatsFacade;
		}

		virtual IEffectsFacade * API_CALL getEffectsFacade()
		{
			return effectsFacade;
		}

		virtual ITexture * API_CALL createTexture()
		{
			return new Texture();
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

		virtual void API_CALL draw(int effect, int vertexFormat, void *vertices, void *indices, int primitivesCount)
		{
			graphicsDevice->renderState.setEffect(effectsFacade->Effects[effect]);
			graphicsDevice->drawPrimitives(vertexFormatsFacade->Formats[vertexFormat], vertices, (unsigned short *)indices, primitivesCount);
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