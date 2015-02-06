// llge.cpp : Defines the exported functions for the DLL application.
//

#include "llge.h"
#include "..\core\core.h"
#include "..\graphics\graphics.h"

#include <limits>

using namespace graphics;

namespace llge
{
	class RenderSystem : public ITestRenderSystem
	{
	private:
		GraphicsDevice * _graphicsDevice;
		core::MatrixContainer *projectionContatiner;
		TextureImage2d _textures[16];
		TextureRenderTarget2d _lightMap;
	public:
		static int poolSize() { return 1; }

		RenderSystem();
		virtual ~RenderSystem();
		virtual void API_CALL viewport(int width, int height);
		virtual void API_CALL create();
		virtual void API_CALL render();
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose();
	};

	class Factory : public IFactory
	{
	public:
		static int poolSize() { return 1; }

		virtual ITestRenderSystem * API_CALL createRenderSystem();
		virtual void API_CALL dispose();
	};

	RenderSystem::RenderSystem()
	{
		_graphicsDevice = new GraphicsDevice();;
		//_graphicsDevice = core::Allocator::create<GraphicsDevice>();;
		projectionContatiner = new core::MatrixContainer(core::Matrix::identity);
	}

	RenderSystem::~RenderSystem()
	{
		//core::Allocator::release<GraphicsDevice>(_graphicsDevice);
		delete _graphicsDevice;
		delete projectionContatiner;
	}
	
	void API_CALL RenderSystem::viewport(int width, int height)
	{
		_graphicsDevice->setViewport(0, 0, width, height);
	}


	void API_CALL RenderSystem::create()
	{
		Effects::create();
		VertexFormats::create();
		_lightMap.create(256, 256);
		for (int i = 0; i < 16; i++)
		{
			_textures[i].create();
			_textures[i].setData(TextureLoader::instance()->loadImage2dData(i));
		}
	}

	struct PositionColor
	{
		PositionColor(float x, float y, float z, unsigned int color) : X(x), Y(y), Z(z), Color(color)
		{
		}
		float X;
		float Y;
		float Z;
		unsigned int Color;
	};

	struct PositionTextureColor
	{
		PositionTextureColor(float x, float y, float z, unsigned short u, unsigned short v, unsigned int color) : X(x), Y(y), Z(z), U(u), V(v), Color(color)
		{
		}
		float X;
		float Y;
		float Z;
		unsigned short U;
		unsigned short V;
		unsigned int Color;
	};
	PositionTextureColor vertexBuffers0[][4] =
	{
		{
			PositionTextureColor(-1.0f, -0.5f, 0.5f, 0, 0, 0xff0000ff),
			PositionTextureColor(-0.5f, -0.5f, 0.5f, 0, 0xffff, 0xffff0000),
			PositionTextureColor(-0.5f, 0.5f, 0.5f, 0xffff, 0xffff, 0xff00ff00),
			PositionTextureColor(-1.0f, 0.5f, 0.5f, 0xffff, 0, 0xffffffff),
		},
		{
			PositionTextureColor(-0.5f, -0.5f, 0.5f, 0, 0, 0xff0000ff),
			PositionTextureColor(0.0f, -0.5f, 0.5f, 0, 0xffff, 0xffff0000),
			PositionTextureColor(0.0f, 0.5f, 0.5f, 0xffff, 0xffff, 0xff00ff00),
			PositionTextureColor(-0.5f, 0.5f, 0.5f, 0xffff, 0, 0xffffffff),
		},
		{
			PositionTextureColor(0.0f, -0.5f, 0.5f, 0, 0, 0xff0000ff),
			PositionTextureColor(0.5f, -0.5f, 0.5f, 0, 0xffff, 0xffff0000),
			PositionTextureColor(0.5f, 0.5f, 0.5f, 0xffff, 0xffff, 0xff00ff00),
			PositionTextureColor(0.0f, 0.5f, 0.5f, 0xffff, 0, 0xffffffff),
		},
		{
			PositionTextureColor(0.5f, -0.5f, 0.5f, 0, 0, 0xff0000ff),
			PositionTextureColor(1.0f, -0.5f, 0.5f, 0, 0xffff, 0xffff0000),
			PositionTextureColor(1.0f, 0.5f, 0.5f, 0xffff, 0xffff, 0xff00ff00),
			PositionTextureColor(0.5f, 0.5f, 0.5f, 0xffff, 0, 0xffffffff),
		}
	};

	PositionTextureColor vertexBuffers1[][4] =
	{
		{
			PositionTextureColor(-0.5f, -1.0f, 0.5f, 0, 0, 0xffffffff),
			PositionTextureColor(-0.5f, 0.0f, 0.5f, 0, 0xffff, 0xffffffff),
			PositionTextureColor(0.5f, 0.0f, 0.5f, 0xffff, 0xffff, 0xffffffff),
			PositionTextureColor(0.5f, -1.0f, 0.5f, 0xffff, 0, 0xffffffff),
		},
		{
			PositionTextureColor(-0.5f, 0.0f, 0.5f, 0, 0, 0xffffffff),
			PositionTextureColor(-0.5f, 1.0f, 0.5f, 0, 0xffff, 0xffffffff),
			PositionTextureColor(0.5f, 1.0f, 0.5f, 0xffff, 0xffff, 0xffffffff),
			PositionTextureColor(0.5f, 0.0f, 0.5f, 0xffff, 0, 0xffffffff),
		}
	};

	unsigned short indexBuffer[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	void API_CALL RenderSystem::render()
	{
#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "render_call");
#endif
		UniformValues::projection()->setValue(*projectionContatiner);
		
		_graphicsDevice->setRenderTarget(&_lightMap);
		_graphicsDevice->setClearState(0x7784ff, 1.0f);
		_graphicsDevice->clear();		
		_graphicsDevice->renderState.setEffect(Effects::textureColor());
		for (int i = 0; i < 4; i++)
		{
			UniformValues::texture()->setValue(_textures[5 + i].getHandle());
			_graphicsDevice->drawPrimitives(VertexFormats::positionTextureColor(), vertexBuffers0[i], indexBuffer, 2);
		}
		_graphicsDevice->setRenderTarget(0);
		
		_graphicsDevice->setClearState(0x7784aa, 1.0f);
		_graphicsDevice->clear();
		_graphicsDevice->renderState.setEffect(Effects::textureColor());
		UniformValues::texture()->setValue(_lightMap.getHandle());
		_graphicsDevice->drawPrimitives(VertexFormats::positionTextureColor(), vertexBuffers1[0], indexBuffer, 2);
		UniformValues::texture()->setValue(_textures[6].getHandle());
		_graphicsDevice->drawPrimitives(VertexFormats::positionTextureColor(), vertexBuffers1[1], indexBuffer, 2);
	}

	void API_CALL RenderSystem::cleanup()
	{
		Effects::cleanup();
		_lightMap.cleanup();
		for (int i = 0; i < 16; i++)
		{
			_textures[i].cleanup();
		}
	}

	void API_CALL RenderSystem::dispose()
	{
		delete this;
		//core::Allocator::release<RenderSystem>(this);
	}
	
	ITestRenderSystem * API_CALL Factory::createRenderSystem()
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

		return new RenderSystem();
		//return core::Allocator::create<RenderSystem>();
	}

	void API_CALL Factory::dispose()
	{
		delete this;
		//core::Allocator::release<Factory>(this);
	}

}

extern "C" DLLEXPORT  llge::IFactory * API_CALL createFactory()
{
	return new  llge::Factory();
	//return core::Allocator::create<Factory>();
}
