#include "ZomboGame.h"
#include "../core/core.h"
#include "../graphics/graphics.h"

namespace zombo
{
	void ZomboGame::setRootPath(String rootPath)
	{
		_rootPath = rootPath;
	}

	void ZomboGame::load(int w, int h)
	{
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		resources::ContentManager::Default.open();
		_contentBlock.setRoot(_rootPath);
		
		_contentBlock.enqueueResource("Content/character/character{n}.atlas");	
	}

	void ZomboGame::update(int w, int h, float ellapsedTime)
	{
		if (!_contentBlock.isLoaded())
		{
			_contentBlock.update();
		}
		else
		{
			if (_aniamtion == 0)
			{
				_aniamtion = _contentBlock.createAnimation("Content/character/character0.atlas/Attack__{n:000}.png");
			}
		}
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
