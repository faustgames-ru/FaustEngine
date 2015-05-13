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
	
	SpineAnimationState::SpineAnimationState(SpineAnimationStateData *data) : _animation(0)
	{
		_spAnimationState = spAnimationState_create((spAnimationStateData*)data->getStateData());
		((spAnimationState*)_spAnimationState)->listener = animationStateListener;
		((spAnimationState*)_spAnimationState)->rendererObject = this;
	}
	
	SpineAnimationState::~SpineAnimationState()
	{
		cleanup();
	}

	void spAnimationState_dummy_update(spAnimationState* self, float delta) {
		int i;
		float previousDelta;
		delta *= self->timeScale;
		for (i = 0; i < self->tracksCount; ++i) {
			spTrackEntry* current = self->tracks[i];
			if (!current) continue;
			current->time += delta * current->timeScale;
			current->lastTime = 0;
		}
	}

	void SpineAnimationState::setAnimation(SpineSkeletonAnimation *animation, bool loop, bool normalize)
	{
		_animation = animation->getAnimation();
		spAnimationState_setAnimation((spAnimationState*)_spAnimationState, 0, (spAnimation *)_animation, loop);		
		if (normalize)
		{
			_timeNormalized = _prevTime / ((spAnimation *)_animation)->duration;
			spAnimationState_dummy_update((spAnimationState*)_spAnimationState, _timeNormalized * ((spAnimation *)_animation)->duration);
		}
	}
	
	void SpineAnimationState::addAnimation(SpineSkeletonAnimation *animation, bool loop, float delay)
	{
		_animation = animation->getAnimation();
		spAnimationState_addAnimation((spAnimationState*)_spAnimationState, 0, (spAnimation *)_animation, loop, delay);
	}
	
	void API_CALL SpineAnimationState::update(float delta)
	{
		spAnimationState* state = (spAnimationState*)_spAnimationState;		
		if ((_animation)&&(state->tracksCount > 0))
		{
			_prevTime = state->tracks[0]->lastTime;
			_time = state->tracks[0]->time;
		}
		spAnimationState_update((spAnimationState*)_spAnimationState, delta);
	}
	

	void API_CALL SpineAnimationState::apply(llge::ISpineSkeleton *skeleton)
	{
		apply((SpineSkeleton *)skeleton->getNativeInstance());
	}

	void API_CALL SpineAnimationState::setAnimation(llge::ISpineAnimation* animation, bool loop, bool normalize)
	{
		setAnimation((SpineSkeletonAnimation *)animation->getNativeInstance(), loop, normalize);
	}

	void API_CALL SpineAnimationState::addAnimation(llge::ISpineAnimation* animation, bool loop, float delay)
	{
		addAnimation((SpineSkeletonAnimation *)animation->getNativeInstance(), loop, delay);
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