#include "ZomboEditor.h"
#include "../../graphics/GraphicsDevice.h"
#include "../../graphics/pipelines/RenderService.h"
#include "../../graphics/pipelines/RenderVertex.h"
#include "../../graphics/pipelines/UniformsConfig.h"
#include "../../core/Environment.h"
#include "../../content/content.h"
#include "../../game/ComponentsFactory.h"
#include "../../game/input/Mouse.h"
#include "components/grid.h"


namespace zombo
{
	ZomboEditor::ZomboEditor() : _loadedScene(nullptr), _updateScene(nullptr)
	{
		_content = new content::ContentBlock();
	}

	void ZomboEditor::setRootPath(String rootPath)
	{
		_content->setRoot(rootPath);
	}

	void ZomboEditor::updateMouse(int rawX, int rawY, int windowX, int windowY, uint buttons)
	{
		game::Mouse::Default.update(rawX, rawY, windowX, windowY, buttons);
	}

	void ZomboEditor::updateEnvironment(int w, int h, float ellapsedTime)
	{
		_viewport.update(w, h);
		if (ellapsedTime > 0.03f)
		{
			// slow motion on fps drop
			ellapsedTime = 0.03f;
		}
		core::Environment::update(ellapsedTime);
	}

	void ZomboEditor::load()
	{
		resources::ContentManager::Default.startLoad();
		_content->enqueueResource("zombo.game")->setLoadedCallback(this, &ZomboEditor::gameLoaded);

		game::ComponentsFactory::Default.addConstructor<Grid>("grid");

		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE
	}

	void ZomboEditor::update()
	{
		if (_loadedScene != nullptr)
		{
			if (!_content->isLoading())
			{
				_updateScene = _loadedScene;
				_loadedScene = nullptr;
				_updateScene->loaded();
			}
		}

		if (_updateScene == nullptr)
		{
			return;
		}
		_updateScene->camera()->viewport = &_viewport;
		_updateScene->camera()->update();
		_updateScene->update();
	}

	float _time = 0;

	void ZomboEditor::render()
	{
		_content->update();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, _viewport.w, _viewport.h);
		graphics::GraphicsDevice::Default.clear();
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);

		graphics::RenderService::Default.applyPipelines();
	}

	void ZomboEditor::release()
	{
		delete this;
	}

	void ZomboEditor::gameLoaded(content::ContentStatus* status)
	{
		content::ContentJson* game = status->asJson();
		const char* startupScene = (*game->value->asObject())["startup_scene"]->asString();
		_content->enqueueResource(startupScene)->setLoadedCallback(this, &ZomboEditor::sceneFileLoaded);
	}

	void ZomboEditor::sceneFileLoaded(content::ContentStatus* status)
	{
		_loadedScene = new game::Scene();
		game::LoadArgs args;
		args.scene = _loadedScene;
		args.content = _content;
		args.value = status->asJson()->value;
		_loadedScene->enqueueResources(args);
	}

	extern "C" DLLEXPORT IZomboEditor* API_CALL createZomboEditor()
	{
		return new ZomboEditor();
	}
}
