#include "SpineSkeleton.h"
#include "SpineAnimationState.h"
#include "SpineAnimationStateData.h"
#include "SpineSkeletonResource.h"
#include "SpineSkeletonAnimation.h"

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
	void animationStateListener(spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount)
	{
		SpineAnimationState* animationState = (SpineAnimationState *)state->rendererObject;
		if (animationState)
		{
			//animationState->listenAnimationEvent(trackIndex, type, event, loopCount);
		}
	}
	
	SpineAnimationState::SpineAnimationState(SpineAnimationStateData *data)
	{
		_spAnimationState = spAnimationState_create((spAnimationStateData*)data->getStateData());
		((spAnimationState*)_spAnimationState)->listener = animationStateListener;
		((spAnimationState*)_spAnimationState)->rendererObject = this;
	}
	
	SpineAnimationState::~SpineAnimationState()
	{
		cleanup();
	}

	void SpineAnimationState::setAnimation(SpineSkeletonAnimation *animation, bool loop)
	{
		spAnimationState_setAnimation((spAnimationState*)_spAnimationState, 0, (spAnimation *)animation->getAnimation(), loop);
	}
	
	void API_CALL SpineAnimationState::update(float delta)
	{
		spAnimationState_update((spAnimationState*)_spAnimationState, delta);
	}
	

	void API_CALL SpineAnimationState::apply(llge::ISpineSkeleton *skeleton)
	{
		apply((SpineSkeleton *)skeleton->getNativeInstance());
	}

	void API_CALL SpineAnimationState::setAnimation(llge::ISpineAnimation* animation, bool loop)
	{
		setAnimation((SpineSkeletonAnimation *)animation->getNativeInstance(), loop);
	}

	void API_CALL SpineAnimationState::dispose()
	{
		delete this;
	}

	void SpineAnimationState::apply(SpineSkeleton* skeleton)
	{
		spAnimationState_apply((spAnimationState*)_spAnimationState, (spSkeleton*)skeleton->getSkeleton());
	}

	
	void SpineAnimationState::cleanup()
	{
		if (_spAnimationState)
		{
			spAnimationState_dispose((spAnimationState*)_spAnimationState);
			_spAnimationState = 0;
		}
	}

	//void AnimationState::listenAnimationEvent(int trackIndex, spEventType type, spEvent* event, int loopCount)
	//{
	//}


}