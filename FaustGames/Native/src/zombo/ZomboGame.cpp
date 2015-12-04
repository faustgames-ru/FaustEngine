#include "ZomboGame.h"
#include "../core/core.h"
#include "../graphics/graphics.h"
#include "../graphics/EffectsBasic.h"
#include "../graphics/VertexFormatsBasic.h"

namespace zombo
{
	void ZomboGame::setRootPath(String rootPath)
	{
		_rootPath = rootPath;
	}

	void ZomboGame::load(int w, int h)
	{
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.renderState = graphics::RenderState(); /// ???
		graphics::GraphicsDevice::Default.renderState.init();
		graphics::GraphicsDevice::Default.create();
		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE
	}

	void ZomboGame::update(int w, int h, float ellapsedTime)
	{
		_zomboWorld.update(ellapsedTime);
	}

	void ZomboGame::render(int w, int h, float ellapsedTime)
	{
		graphics::GraphicsDevice::Default.setViewport(0, 0, w, h);
		graphics::GraphicsDevice::Default.clear();
	}

	void ZomboGame::release()
	{
		delete this;
	}

	extern "C" DLLEXPORT IZomboGame * API_CALL createZomboGame()
	{
		return new ZomboGame();
	}

	extern "C" DLLEXPORT void API_CALL initZomboRenderContext()
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
}
