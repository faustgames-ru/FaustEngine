#include "ZomboScene.h"

namespace zombo
{
	ZomboScene::ZomboScene()
	{
		stateController = new StateController();
		stateLoading = new SceneStateLoading(this);
		stateShow = new SceneStateShow(this);
		stateIdle = new SceneStateIdle(this);
		stateHide = new SceneStateHide(this);
		stateUnloading = new SceneStateUnloading(this);
		stateController->setState(stateLoading);
	}

	ZomboScene::~ZomboScene()
	{
		delete stateController;
		delete stateLoading;
		delete stateShow;
		delete stateIdle;
		delete stateHide;
		delete stateUnloading;
	}

	void ZomboScene::update(float ellapsedTime) const
	{
		stateController->update(ellapsedTime);
	}

	SceneState::SceneState(ZomboScene* sceneValue)
		: scene(sceneValue)
	{
	}

	SceneStateLoading::SceneStateLoading(ZomboScene* sceneValue)
		: SceneState(sceneValue)
	{		
	}

	void SceneStateLoading::activated()
	{
	}

	void SceneStateLoading::deactivated()
	{
	}

	void SceneStateLoading::update(float ellapsedTime)
	{
		if (scene->content.isLoaded())
			scene->stateController->setState(scene->stateShow);
	}

	SceneStateShow::SceneStateShow(ZomboScene* sceneValue)
		: SceneState(sceneValue)
	{
	}

	SceneStateIdle::SceneStateIdle(ZomboScene* sceneValue)
		: SceneState(sceneValue)
	{
	}

	SceneStateHide::SceneStateHide(ZomboScene* sceneValue)
		: SceneState(sceneValue)
	{
	}

	SceneStateUnloading::SceneStateUnloading(ZomboScene* sceneValue)
		: SceneState(sceneValue)
	{
	}
}
