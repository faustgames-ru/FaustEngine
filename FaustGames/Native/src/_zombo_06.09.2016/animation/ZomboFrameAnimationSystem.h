#ifndef ZOMBO_FRAME_ANIMATION_SYSTEM_H
#define ZOMBO_FRAME_ANIMATION_SYSTEM_H

#include "../zombo_classes.h"
#include "ZomboFrameAnimation.h"

namespace zombo
{
	class ZomboFrameAnimationsAbstractBlock
	{
	public:
		virtual ~ZomboFrameAnimationsAbstractBlock();
		virtual void update() = 0;
	};

	template<int framesCount>
	class ZomboFrameAnimationsBlock : ZomboFrameAnimationsAbstractBlock
	{
	public:
		std::vector<ZomboFrameAnimation<framesCount>> animations;
		virtual void update() OVERRIDE;
	};

	class ZomboFrameAnimationSystem
	{
	public:
		std::vector<ZomboFrameAnimationsAbstractBlock> animations;
		inline void update();		
	};
}

#endif /* ZOMBO_FRAME_ANIMATION_SYSTEM_H */