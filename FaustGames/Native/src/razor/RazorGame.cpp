#include "RazorGame.h"
#include <glew.h>
#include "../graphics/GraphicsDevice.h"
#include "../graphics/pipelines/RenderService.h"
#include "../graphics/pipelines/RenderVertex.h"
#include "../graphics/pipelines/UniformsConfig.h"
#include "../core/Environment.h"
#include "../content/content.h"
#include "../game/ComponentsFactory.h"
#include "components/Spaceship.h"
#include "components/Grid.h"


namespace razor
{
	RazorGame::RazorGame(): _loadedScene(nullptr), _updateScene(nullptr)
	{
		_content = new content::ContentBlock();
	}

	void RazorGame::setRootPath(String rootPath)
	{
		_content->setRoot(rootPath);
	}

	void RazorGame::updateEnvironment(int w, int h, float ellapsedTime)
	{
		_viewport.update(w, h);
		core::Environment::update(ellapsedTime);
	}

	void RazorGame::load()
	{
		resources::ContentManager::Default.startLoad();
		_content->enqueueResource("razor.game")->setLoadedCallback(this, &RazorGame::gameLoaded);

		game::ComponentsFactory::Default.addConstructor<Spaceship>("spaceship");
		game::ComponentsFactory::Default.addConstructor<Grid>("grid");

		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE
	}

	void RazorGame::update()
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
		_updateScene->camera()->aspect = _viewport.getAspect();
		_updateScene->update();
	}

	float _time = 0;

	void RazorGame::render()
	{		
		_content->update();
		
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, _viewport.w, _viewport.h);
		graphics::GraphicsDevice::Default.clear();
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
	
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;
		graphics::RenderService::Default.applyPipelines();
	}

	void RazorGame::release()
	{
		delete this;
	}

	void RazorGame::gameLoaded(content::ContentStatus* status)
	{
		content::ContentJson* game = status->asJson();		
		const char* startupScene = (*game->value->asObject())["startup_scene"]->asString();
		_content->enqueueResource(startupScene)->setLoadedCallback(this, &RazorGame::sceneFileLoaded);
	}

	void RazorGame::sceneFileLoaded(content::ContentStatus* status)
	{
		_loadedScene = new game::Scene();
		game::LoadArgs args;
		args.content = _content;
		args.value = status->asJson()->value;
		_loadedScene->enqueueResources(args);
	}


	extern "C" DLLEXPORT IRazorGame* API_CALL createRazorGame()
	{
		return new RazorGame();
	}
}
