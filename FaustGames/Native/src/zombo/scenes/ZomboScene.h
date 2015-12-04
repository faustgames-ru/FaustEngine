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

	class ZomboScene
	{
	public:
		ZomboContentBlock content;
		SceneStateLoading *stateLoading;
		SceneStateShow *stateShow;
		SceneStateIdle *stateIdle;
		SceneStateHide *stateHide;
		SceneStateUnloading *stateUnloading;
		StateController *stateController;
		ZomboScene();
		~ZomboScene();
		void update(float ellapsedTime) const;
	private:
	};

	class SceneState : public StateBase
	{
	public:
		ZomboScene *scene;
		SceneState(ZomboScene *sceneValue);
	};

	// display black screen 
	class SceneStateLoading : public SceneState
	{
	public:
		SceneStateLoading(ZomboScene *sceneValue);
		virtual void activated() OVERRIDE;
		virtual void deactivated() OVERRIDE;
		virtual void update(float ellapsedTime) OVERRIDE;
	};

	// fade black screen hide
	class SceneStateShow : public SceneState
	{
	public:
		SceneStateShow(ZomboScene* sceneValue);
	};

	class SceneStateIdle : public SceneState
	{
	public:
		SceneStateIdle(ZomboScene *sceneValue);
	};

	// fade black screen show
	class SceneStateHide : public SceneState
	{
	public:
		SceneStateHide(ZomboScene *sceneValue);
	};

	// display black screen 
	class SceneStateUnloading : public SceneState
	{
	public:
		SceneStateUnloading(ZomboScene *sceneValue);
	};
}

#endif /*ZOMBO_SCENE_H*/
