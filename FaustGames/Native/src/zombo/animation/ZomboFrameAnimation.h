#ifndef ZOMBO_FRAME_ANIMATION_H
#define ZOMBO_FRAME_ANIMATION_H

#include "../zombo_classes.h"
#include "../content/ZomboAtlas.h"

namespace zombo
{
	class ZomboFrameAnimation
	{
	public:
		static ZomboComponentTypes::e ComponentType;
		static ZomboFrameAnimation * create();
		ZomboFrameAnimation();
		~ZomboFrameAnimation();
		void dispose() const;
		void setAnimation(ZomboContentFrameAnimation* value);
	private:
		ulong _startTime;
		ZomboContentImage* _actualImage;
		ZomboContentFrameAnimation* _animation;
	};
}

#endif /* ZOMBO_FRAME_ANIMATION_H */