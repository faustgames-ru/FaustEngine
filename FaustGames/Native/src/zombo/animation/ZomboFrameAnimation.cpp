#include "ZomboFrameAnimation.h"

namespace zombo
{
	ZomboComponentTypes::e ZomboFrameAnimation::ComponentType(ZomboComponentTypes::FrameAnimation);


	ZomboFrameAnimation* ZomboFrameAnimation::create()
	{
		return new ZomboFrameAnimation();
	}

	ZomboFrameAnimation::ZomboFrameAnimation(): _actualImage(nullptr), _animation(nullptr), _startTime(0xffffffff)
	{
	}

	ZomboFrameAnimation::~ZomboFrameAnimation()
	{
	}

	void ZomboFrameAnimation::dispose() const
	{
		delete this;
	}

	void ZomboFrameAnimation::setAnimation(ZomboContentFrameAnimation* value)
	{
		_actualImage = nullptr;
		_animation = value;
	}
}
