#include "ZomboGame.h"
#include "../core/core.h"
#include "../graphics/graphics.h"
#include "../graphics/EffectsBasic.h"
#include "../graphics/VertexFormatsBasic.h"
#include "content/serialization/ZomboValue.h"
#include "content/serialization/ZomboObject.h"
#include "content/serialization/ZomboArray.h"
#include "content/serialization/ZomboSerializer.h"

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

		resources::ContentManager::Default.open();
		_contentBlock.setRoot(_rootPath);
		_contentBlock.enqueueResource("Content/character/character{n}.atlas");

		resources::ContentManager content = resources::ContentManager::Default;
		std::string fullPath = _rootPath + "Content/test.scene";
		if (resources::ContentProvider::existContent(fullPath.c_str()))
		{
			resources::ContentProvider::openContent(fullPath.c_str());
			char * jsonString = static_cast<char *>(content.getBuffer());
			int len = 0;
			const int pageSize = 256 * 1024;
			int count = 0;
			while ((count = resources::ContentProvider::read(jsonString + len, pageSize)) > 0)
			{
				len += count;
			}

			ZomboValue *sceneValue = ZomboSerializer::deserializeFromJson(jsonString);
			ZomboObject * sceneObject = sceneValue->asObject();
			ZomboArray *resources = (*sceneObject)["resources"]->asArray();
			int resourcesCount = resources->size();
		}

	}

	void ZomboGame::update(int w, int h, float ellapsedTime)
	{
		_contentBlock.update();
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
