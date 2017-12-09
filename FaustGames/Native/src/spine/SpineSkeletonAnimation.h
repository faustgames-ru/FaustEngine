#ifndef SPINE_SKELETON_ANIMATION_H
#define SPINE_SKELETON_ANIMATION_H

#include "spine_classes.h"

namespace spine
{
	class SpineAnimationEvent : public llge::ISpineAnimationEvent
	{
	public:
		virtual IntPtr API_CALL getName();
		virtual float API_CALL getTime();
		SpineAnimationEvent(const char* name, float time);
	private:
		std::string _name;
		float _time;
	};


	class SpineSkeletonAnimation : public llge::ISpineAnimation
	{
	public:
		SpineSkeletonAnimation(void *animation);
		~SpineSkeletonAnimation();
		void *getAnimation();
		std::string name() const;
		virtual IntPtr API_CALL getNativeInstance();
		virtual IntPtr API_CALL getName();
		virtual float API_CALL getDuration();

		virtual llge::ISpineAnimationEvent* API_CALL getEvent(int i);
		virtual int API_CALL getEventsCount();
	protected:
	private:
		std::vector<SpineAnimationEvent* > _events;
		void *_spAnimation;
	};
}

#endif /*SPINE_SKELETON_ANIMATION_H*/