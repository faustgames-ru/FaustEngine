#ifndef SPINE_SKELETON_RESOURCE_H
#define SPINE_SKELETON_RESOURCE_H

#include "spine_classes.h"

namespace spine
{
	class SpineSkeletonResource : public llge::ISpineResource
	{
	public:
		void *getSkeletonData();
		SpineSkeletonAnimation* getAnimation(int i);
		int getAnimationsCount();

		virtual void API_CALL load(String atlasText, String jsonText);
		virtual void API_CALL unLoad();
		virtual llge::ISpineAnimation* API_CALL getSpineAnimation(int i);
		virtual int API_CALL getSpineAnimationsCount();
		virtual llge::ISpineSkeleton* API_CALL createSkeleton();
		virtual llge::ISpineAnimationStateData* API_CALL createStateData();
		virtual void API_CALL dispose();
	protected:
	private:
		void * _spAtlas;
		void *_spSkeletonJson;
		void *_spSkeletonData;
		std::vector<SpineSkeletonAnimation *> _animations;
	};
}

#endif /*SPINE_SKELETON_RESOURCE_H*/