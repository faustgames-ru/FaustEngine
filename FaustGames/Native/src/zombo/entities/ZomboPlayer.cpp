#include "ZomboPlayer.h"
#include "../common/ZomboTransfom.h"
#include "../common/ZomboBounds.h"
#include "../render/ZomboRenderSprite.h"
#include "../animation/ZomboFrameAnimation.h"

namespace zombo
{
	ZomboPlayer::ZomboPlayer()
	{
		createComponent<ZomboBounds>();
		createComponent<ZomboTransfom>();
		createComponent<ZomboRenderSprite>();
		createComponent<ZomboFrameAnimation>();
	}

	ZomboPlayer::~ZomboPlayer()
	{
		disposeComponent<ZomboBounds>();
		disposeComponent<ZomboTransfom>();
		disposeComponent<ZomboRenderSprite>();
		disposeComponent<ZomboFrameAnimation>();
	}
}
