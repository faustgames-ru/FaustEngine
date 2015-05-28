#ifndef SPINE_SKELETON_RESOURCE_H
#define SPINE_SKELETON_RESOURCE_H

#include "spine_classes.h"

namespace spine
{
	class SpineEvent : public llge::ISpineEvent
	{
	public:
		virtual IntPtr API_CALL getName();
		SpineEvent(const char * name);
	private:
		std::string _name;
	};

	class SpineSkeletonResource : public llge::ISpineResource
	{
	public:
		void *getSkeletonData();
		SpineSkeletonAnimation* getAnimation(int i);
		int getAnimationsCount();
		SpineEvent* getEvent(int i);
		int getEventsCount();

		virtual void API_CALL load(String atlasText, String jsonText, String dir);
		virtual void API_CALL unLoad();
		virtual llge::ISpineAnimation* API_CALL getSpineAnimation(int i);
		virtual int API_CALL getSpineAnimationsCount();
		virtual llge::ISpineEvent* API_CALL getSpineEvent(int i);
		virtual int API_CALL getSpineEventsCount();
		virtual llge::ISpineSkeleton* API_CALL createSkeleton(void *floatMatrix);
		virtual llge::ISpineAnimationStateData* API_CALL createStateData();
		virtual void API_CALL dispose();
	protected:
	private:
		void * _spAtlas;
		void *_spSkeletonJson;
		void *_spSkeletonData;
		std::vector<SpineSkeletonAnimation *> _animations;
		std::vector<SpineEvent *> _events;
	};
}

#endif /*SPINE_SKELETON_RESOURCE_H*/