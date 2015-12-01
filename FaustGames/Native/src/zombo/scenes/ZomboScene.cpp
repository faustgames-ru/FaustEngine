#include "ZomboScene.h"

namespace zombo
{
	Scene::Scene()
	{
		stateController = new StateController();
		stateLoading = new SceneStateLoading(this);
		stateShow = new SceneStateShow(this);
		stateIdle = new SceneStateIdle(this);
		stateHide = new SceneStateHide(this);
		stateUnloading = new SceneStateUnloading(this);
	}

	Scene::~Scene()
	{
		delete stateController;
		delete stateLoading;
		delete stateShow;
		delete stateIdle;
		delete stateHide;
		delete stateUnloading;
	}

	void Scene::update(float ellapsedTime) const
	{
		stateController->update(ellapsedTime);
	}

	SceneState::SceneState(Scene* sceneValue)
		: scene(sceneValue)
	{
	}

	SceneStateLoading::SceneStateLoading(Scene* sceneValue)  
		: SceneState(sceneValue)
	{		
	}

	SceneStateShow::SceneStateShow(Scene* sceneValue)
		: SceneState(sceneValue)
	{
	}

	SceneStateIdle::SceneStateIdle(Scene* sceneValue)
		: SceneState(sceneValue)
	{
	}

	SceneStateHide::SceneStateHide(Scene* sceneValue)
		: SceneState(sceneValue)
	{
	}

	SceneStateUnloading::SceneStateUnloading(Scene* sceneValue)
		: SceneState(sceneValue)
	{
	}
}
