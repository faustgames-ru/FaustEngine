#include "ZomboPlayer.h"
#include "../common/ZomboTransfom.h"
#include "../common/ZomboBounds.h"
#include "../render/ZomboRenderSprite.h"
#include "../animation/ZomboFrameAnimation.h"
#include "../behaviors/ZomboBehaviorPlayer.h"

namespace zombo
{
	ZomboPlayer::ZomboPlayer()
	{
		createComponent<ZomboBounds>();
		createComponent<ZomboTransfom>();
		createComponent<ZomboRenderSprite>();
		createComponent<ZomboFrameAnimation>();		
		createComponent<ZomboBehaviors>()->
			addBehavior<ZomboBehaviorPlayer>();

	}

	ZomboPlayer::~ZomboPlayer()
	{
		disposeComponent<ZomboBounds>();
		disposeComponent<ZomboTransfom>();
		disposeComponent<ZomboRenderSprite>();
		disposeComponent<ZomboFrameAnimation>();
		disposeComponent<ZomboBehaviors>();
	}
}
