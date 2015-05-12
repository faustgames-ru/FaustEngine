#ifndef SPINE_ANIMATION_STATE_DATA_H
#define SPINE_ANIMATION_STATE_DATA_H

#include "spine_classes.h"

namespace spine
{
	class SpineAnimationStateData : public llge::ISpineAnimationStateData
	{
	public:
		SpineAnimationStateData(SpineSkeletonResource* resource);
		~SpineAnimationStateData();
		void setMix(SpineSkeletonAnimation *from, SpineSkeletonAnimation *to, float time);
		void cleanup();
		void *getStateData();

		virtual void API_CALL setMix(llge::ISpineAnimation *from, llge::ISpineAnimation *to, float time);
		virtual llge::ISpineAnimationState* API_CALL createState();
		virtual void API_CALL dispose();
	protected:
	private:
		void initFromResource(SpineSkeletonResource* resource);
		void* _spAnimationStateData;
	};
}

#endif /*SPINE_ANIMATION_STATE_DATA_H*/