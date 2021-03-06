#ifndef SPINE_ANIMATION_STATE_H
#define SPINE_ANIMATION_STATE_H

#include "spine_classes.h"

namespace spine
{
	struct SpineEventsBuffer
	{
		int* EventsIndices;
		int EventsIndicesCount;
		int EventsIndicesLimit;
		SpineEventsBuffer(int limit)
		{
			EventsIndicesCount = 0;
			EventsIndicesLimit = limit;
			EventsIndices = new int[limit];
		}
		~SpineEventsBuffer()
		{
			delete [] EventsIndices;
		}
	};

	class SpineAnimationState : public llge::ISpineAnimationState
	{
	public:
		SpineAnimationState(SpineAnimationStateData *data);
		~SpineAnimationState();
		void setAnimation(SpineSkeletonAnimation *animation, int track, bool loop, bool normalize);
		void setAnimation(SpineSkeletonAnimation *animation, bool loop, bool normalize);
		void addAnimation(SpineSkeletonAnimation *animation, bool loop, float delay);
		void apply(SpineSkeleton *skeleton);
		void cleanup();
		
		virtual float API_CALL getTime() OVERRIDE;
		virtual void API_CALL setTime(float time) OVERRIDE;
		virtual void API_CALL update(float delta) OVERRIDE;
		virtual void API_CALL apply(llge::ISpineSkeleton *skeleton) OVERRIDE;
		virtual void API_CALL setAnimation(llge::ISpineAnimation* animation, bool loop, bool normalize) OVERRIDE;
		virtual void API_CALL addAnimation(llge::ISpineAnimation* animation, bool loop, float delay) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
		virtual int API_CALL getSpineEventIndices(IntPtr indices, int limit) OVERRIDE;
		virtual int API_CALL getSpineEventsLimit();
		
		void listenAnimationEvent(int eventIndex);
	protected:
	private:
		void *_spAnimationState;
		float _time;
		float _prevTime;
		float _timeNormalized;
		void* _animation;

		SpineEventsBuffer *_eventsBuffer;
	};
}

#endif /*SPINE_ANIMATION_STATE_H*/