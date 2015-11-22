#ifndef ZOMBO_FRAME_ANIMATION_H
#define ZOMBO_FRAME_ANIMATION_H

#include "../zombo_classes.h"

namespace zombo
{
	template <int framesCount>
	class ZomboFrameAnimation
	{
	public:
		float frameIndex;
		float fps;
		ZomboSpriteImage frames[framesCount];
	};

	typedef ZomboFrameAnimation<10> ZomboFrameAnimation10;
	typedef ZomboFrameAnimation<15> ZomboFrameAnimation15;
	typedef ZomboFrameAnimation<30> ZomboFrameAnimation30;
}

#endif /* ZOMBO_FRAME_ANIMATION_H */