#include "SpineSkeleton.h"
#include "SpineAnimationState.h"
#include "SpineAnimationStateData.h"
#include "SpineSkeletonResource.h"
#include "SpineSkeletonAnimation.h"

#include "spine/Skeleton.h"
#include "spine/Animation.h"
#include "spine/AnimationState.h"
#include "spine/AnimationStateData.h"
#include "spine/SkeletonJson.h"
#include "spine/SkeletonData.h"
#include "spine/Atlas.h"
#include "spine/AtlasAttachmentLoader.h"

namespace spine
{
	void animationStateListener(spAnimationState* state, int trackIndex, spEventType type, spEvent* e, int loopCount)
	{
		SpineAnimationState* animationState = static_cast<SpineAnimationState *>(state->rendererObject);
		if (animationState)
		{
			if (type == SP_ANIMATION_EVENT)
			{
				animationState->listenAnimationEvent(e->data->intValue);
			} 
			else if (type == SP_ANIMATION_END)
			{
				animationState->listenAnimationEvent(-1);
			}
			else if (type == SP_ANIMATION_COMPLETE)
			{
				animationState->listenAnimationEvent(-2);
			}
			else if (type == SP_ANIMATION_START)
			{
				animationState->listenAnimationEvent(-3);
			}
		}
	}
	
	SpineAnimationState::SpineAnimationState(SpineAnimationStateData *data) : _animation(0)
	{
		spAnimationStateData *spData = static_cast<spAnimationStateData*>(data->getStateData());
		_spAnimationState = spAnimationState_create(spData);
		static_cast<spAnimationState*>(_spAnimationState)->listener = animationStateListener;
		static_cast<spAnimationState*>(_spAnimationState)->rendererObject = this;
		_eventsBuffer = new SpineEventsBuffer(spData->skeletonData->eventsCount+3);
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

	void spAnimationState_set_time(spAnimationState* self, float time, float duration) {
		int i;
		float previousDelta;
		time *= self->timeScale;
		time = core::Math::clamp(time, 0, duration);
		for (i = 0; i < self->tracksCount; ++i) {
			spTrackEntry* current = self->tracks[i];
			if (!current) continue;
			current->time = time;
			current->lastTime = 0;
		}
	}

	void SpineAnimationState::setAnimation(SpineSkeletonAnimation *animation, bool loop, bool normalize)
	{
		float duration;
		if (_animation)
		{
			duration = static_cast<spAnimation *>(_animation)->duration;
		}
		else
		{
			duration = static_cast<spAnimation *>(animation->getAnimation())->duration;
		}
		
		_animation = animation->getAnimation();
		_eventsBuffer->EventsIndicesCount = 0;
		spAnimationState_setAnimation(static_cast<spAnimationState*>(_spAnimationState), 0, static_cast<spAnimation *>(_animation), loop);
		if (normalize)
		{
			float prevTime = _prevTime;
			if (prevTime >duration)
			{
				prevTime -= core::Math::trunc(prevTime / duration);
			}
			_timeNormalized = prevTime / duration;
			float time = _timeNormalized * static_cast<spAnimation *>(_animation)->duration;
			spAnimationState_dummy_update(static_cast<spAnimationState*>(_spAnimationState), time);
			_prevTime = time;
		}
		else
		{
			spAnimationState_dummy_update(static_cast<spAnimationState*>(_spAnimationState), 0);
			_prevTime = 0;
		}
	}
	
	void SpineAnimationState::addAnimation(SpineSkeletonAnimation *animation, bool loop, float delay)
	{
		_animation = animation->getAnimation();
		spAnimationState_addAnimation(static_cast<spAnimationState*>(_spAnimationState), 0, static_cast<spAnimation *>(_animation), loop, delay);
	}
	
	void API_CALL SpineAnimationState::update(float delta)
	{
		spAnimationState* state = static_cast<spAnimationState*>(_spAnimationState);		
		if ((_animation != nullptr) && (state->tracksCount > 0) && state->tracks)
		{
			if (state->tracks[0])
			{
				_prevTime = state->tracks[0]->lastTime;
				if (_prevTime < 0)
					_prevTime = 0.0f;
				_time = state->tracks[0]->time;
			}
		}
		spAnimationState_update(static_cast<spAnimationState*>(_spAnimationState), delta);
	}
	

	void API_CALL SpineAnimationState::apply(llge::ISpineSkeleton *skeleton)
	{
		_eventsBuffer->EventsIndicesCount = 0;
		apply(static_cast<SpineSkeleton *>(skeleton->getNativeInstance()));
		static_cast<SpineSkeleton *>(skeleton->getNativeInstance())->updateAabb();
	}

	void API_CALL SpineAnimationState::setAnimation(llge::ISpineAnimation* animation, bool loop, bool normalize)
	{
		setAnimation(static_cast<SpineSkeletonAnimation *>(animation->getNativeInstance()), loop, normalize);
	}

	void API_CALL SpineAnimationState::addAnimation(llge::ISpineAnimation* animation, bool loop, float delay)
	{
		addAnimation(static_cast<SpineSkeletonAnimation *>(animation->getNativeInstance()), loop, delay);
	}
	
	void API_CALL SpineAnimationState::dispose()
	{
		delete this;
	}
		
	int API_CALL SpineAnimationState::getSpineEventIndices(IntPtr indices, int limit)
	{
		int *inds = static_cast<int *>(indices);
		for (int i = 0; i < _eventsBuffer->EventsIndicesCount; i++)
		{
			if (i >= limit) return limit;
			inds[i] = _eventsBuffer->EventsIndices[i];
		}
		return _eventsBuffer->EventsIndicesCount;
	}

	int SpineAnimationState::getSpineEventsLimit()
	{
		return _eventsBuffer->EventsIndicesLimit;
	}

	void SpineAnimationState::listenAnimationEvent(int eventIndex)
	{
		_eventsBuffer->EventsIndices[_eventsBuffer->EventsIndicesCount++] = eventIndex;
	}


	void SpineAnimationState::apply(SpineSkeleton* skeleton)
	{
		spAnimationState_apply(static_cast<spAnimationState*>(_spAnimationState), static_cast<spSkeleton*>(skeleton->getSkeleton()));
	}

	
	void SpineAnimationState::cleanup()
	{
		if (_spAnimationState)
		{
			spAnimationState_dispose(static_cast<spAnimationState*>(_spAnimationState));
			_spAnimationState = 0;
			delete _eventsBuffer;
		}
	}

	float SpineAnimationState::getTime()
	{
		spAnimationState* state = static_cast<spAnimationState*>(_spAnimationState);
		if (_animation != nullptr && state->tracksCount > 0 && state->tracks != nullptr)
		{
			if (state->tracks[0])
			{
				return state->tracks[0]->time;
			}
		}
		return 0.0f;
	}

	void SpineAnimationState::setTime(float time)
	{
		if (_animation != nullptr)
		{
			float duration = static_cast<spAnimation *>(_animation)->duration;
			spAnimationState_set_time(static_cast<spAnimationState*>(_spAnimationState), time, duration);
		}
	}
}