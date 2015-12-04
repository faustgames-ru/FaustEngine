#ifndef ZOMBO_BEHAVIOR_PLAYER_H
#define ZOMBO_BEHAVIOR_PLAYER_H

#include "../zombo_classes.h"
#include "ZomboBehaviors.h"
#include "../animation/ZomboFrameAnimation.h"
#include "../content/ZomboContentBlock.h"

namespace zombo
{
	class ZomboBehaviorPlayerStaticData;

	class ZomboBehaviorPlayer : public IZomboBehavior
	{
	public:
		static ZomboBehaviorPlayerStaticData staticData;
		ZomboBehaviorPlayer();
		virtual ~ZomboBehaviorPlayer();
		virtual void load(ZomboLoadContext *context) OVERRIDE;
		virtual void update(ZomboEntity* entity, float ellapsedTime) OVERRIDE;
	private:

	};

	class ZomboBehaviorPlayerStaticData : IZomboBehaviorStaticData
	{
	public:
		ZomboContentFrameAnimation* idle;
		ZomboContentFrameAnimation* run;
		ZomboContentFrameAnimation* jump;
		ZomboBehaviorPlayerStaticData();
		~ZomboBehaviorPlayerStaticData();
		virtual void load(ZomboLoadContext *context) OVERRIDE;
		virtual void unload() OVERRIDE;
	};
}

#endif /*ZOMBO_BEHAVIOR_H*/
