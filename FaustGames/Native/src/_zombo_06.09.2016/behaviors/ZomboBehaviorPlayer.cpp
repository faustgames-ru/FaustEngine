#include "ZomboBehaviorPlayer.h"
#include "../content/ZomboContentBlock.h"

namespace zombo
{
	ZomboBehaviorPlayer::ZomboBehaviorPlayer()
	{
	}

	ZomboBehaviorPlayer::~ZomboBehaviorPlayer()
	{
	}

	void ZomboBehaviorPlayer::load(ZomboLoadContext *context)
	{
	}

	void ZomboBehaviorPlayer::update(ZomboEntity* entity, float ellapsedTime)
	{
	}

	ZomboBehaviorPlayerStaticData::ZomboBehaviorPlayerStaticData()
		: idle(nullptr), run(nullptr), jump(nullptr)
	{
	}

	ZomboBehaviorPlayerStaticData::~ZomboBehaviorPlayerStaticData()
	{
	}
}
