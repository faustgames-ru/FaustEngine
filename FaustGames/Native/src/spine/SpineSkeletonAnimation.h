#ifndef SPINE_SKELETON_ANIMATION_H
#define SPINE_SKELETON_ANIMATION_H

#include "spine_classes.h"

namespace spine
{
	class SpineSkeletonAnimation : public llge::ISpineAnimation
	{
	public:
		SpineSkeletonAnimation(void *animation);
		void *getAnimation();
		virtual IntPtr API_CALL getNativeInstance();
		virtual IntPtr API_CALL getName();
		virtual float API_CALL getDuration();
	protected:
	private:
		void *_spAnimation;
	};
}

#endif /*SPINE_SKELETON_ANIMATION_H*/