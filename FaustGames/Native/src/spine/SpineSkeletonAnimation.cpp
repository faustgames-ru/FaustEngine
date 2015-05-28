#include "SpineSkeletonAnimation.h"
#include "SpineSkeleton.h"

#include "spine\Skeleton.h"
#include "spine\Animation.h"
#include "spine\AnimationState.h"
#include "spine\AnimationStateData.h"
#include "spine\SkeletonJson.h"
#include "spine\SkeletonData.h"
#include "spine\Atlas.h"
#include "spine\AtlasAttachmentLoader.h"

namespace spine
{
	SpineSkeletonAnimation::SpineSkeletonAnimation(void* animation)
	{
		_spAnimation = animation;
	}
	
	void* SpineSkeletonAnimation::getAnimation()
	{
		return _spAnimation;
	}

	IntPtr API_CALL SpineSkeletonAnimation::getNativeInstance()
	{
		return this;
	}

	IntPtr API_CALL SpineSkeletonAnimation::getName()
	{
		return (IntPtr)((spAnimation *)_spAnimation)->name;
	}

	float API_CALL SpineSkeletonAnimation::getDuration()
	{
		return ((spAnimation *)_spAnimation)->duration;
	}


}