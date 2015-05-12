#ifndef SPINE_ANIMATION_STATE_H
#define SPINE_ANIMATION_STATE_H

#include "spine_classes.h"

namespace spine
{
	class SpineAnimationState : public llge::ISpineAnimationState
	{
	public:
		SpineAnimationState(SpineAnimationStateData *data);
		~SpineAnimationState();
		void setAnimation(SpineSkeletonAnimation *animation, bool loop);
		void apply(SpineSkeleton *skeleton);
		void cleanup();
		
		virtual void API_CALL update(float delta);
		virtual void API_CALL apply(llge::ISpineSkeleton *skeleton);
		virtual void API_CALL setAnimation(llge::ISpineAnimation* animation, bool loop);
		virtual void API_CALL dispose();

	protected:
	private:
		//void static animationStateListener(spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount);
		//void listenAnimationEvent(int trackIndex, spEventType type, spEvent* event, int loopCount);
		void *_spAnimationState;
	};
}

#endif /*SPINE_ANIMATION_STATE_H*/