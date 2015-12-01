#ifndef ZOMBO_SCENE_H
#define ZOMBO_SCENE_H

#include "../zombo_classes.h"
#include "../common/states/ZomboStateController.h"
#include "../content/ZomboContentBlock.h"

namespace zombo
{
	class SceneState;
	class SceneStateLoading;
	class SceneStateShow;
	class SceneStateIdle;
	class SceneStateHide;
	class SceneStateUnloading;

	class Scene
	{
	public:
		ZomboContentBlock content;
		SceneStateLoading *stateLoading;
		SceneStateShow *stateShow;
		SceneStateIdle *stateIdle;
		SceneStateHide *stateHide;
		SceneStateUnloading *stateUnloading;
		StateController *stateController;
		Scene();
		~Scene();
		void update(float ellapsedTime) const;
	private:
	};

	class SceneState : public StateBase
	{
	public:
		Scene *scene;
		SceneState(Scene *sceneValue);
	};

	// display black screen 
	class SceneStateLoading : public SceneState
	{
	public:
		SceneStateLoading(Scene *sceneValue);
	};

	// fade black screen hide
	class SceneStateShow : public SceneState
	{
	public:
		SceneStateShow(Scene* sceneValue);
	};

	class SceneStateIdle : public SceneState
	{
	public:
		SceneStateIdle(Scene *sceneValue);
	};

	// fade black screen show
	class SceneStateHide : public SceneState
	{
	public:
		SceneStateHide(Scene *sceneValue);
	};

	// display black screen 
	class SceneStateUnloading : public SceneState
	{
	public:
		SceneStateUnloading(Scene *sceneValue);
	};
}

#endif /*ZOMBO_SCENE_H*/
