// llge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "graphics.h"
#include "ObjectsPool.h"
#include "GraphicsDevice.h"
#include "Effects.h"
#include "llge.h"

using namespace graphics;

namespace llge
{
	class RenderSystem : public IRenderSystem
	{
	private:
		GraphicsDevice * _graphicsDevice;
	public:
		static int poolSize() { return 1; }

		RenderSystem();
		virtual ~RenderSystem();
		virtual void API_CALL create();
		virtual void API_CALL render();
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose();
	};

	class Factory : public IFactory
	{
	public:
		static int poolSize() { return 1; }

		virtual IRenderSystem * API_CALL createRenderSystem();
		virtual void API_CALL dispose();
	};

	RenderSystem::RenderSystem()
	{
		_graphicsDevice = core::Allocator::create<GraphicsDevice>();;
	}

	RenderSystem::~RenderSystem()
	{
		core::Allocator::release<GraphicsDevice>(_graphicsDevice);
	}
	
	void API_CALL RenderSystem::create()
	{
		Effects::instance()->create();
	}

	void API_CALL RenderSystem::render()
	{
		_graphicsDevice->setClearState(0x7784aa, 1.0f);
		_graphicsDevice->clear();
	}

	void API_CALL RenderSystem::cleanup()
	{
		Effects::instance()->cleanup();
	}

	void API_CALL RenderSystem::dispose()
	{
		core::Allocator::release<RenderSystem>(this);
	}
	
	IRenderSystem * API_CALL Factory::createRenderSystem()
	{
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

		return core::Allocator::create<RenderSystem>();
	}

	void API_CALL Factory::dispose()
	{
		core::Allocator::release<Factory>(this);
	}

	extern "C" DLLEXPORT IFactory * API_CALL createFactory()
	{
		return core::Allocator::create<Factory>();
	}
}