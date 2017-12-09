#include "SpineSkeletonAnimation.h"
#include "SpineSkeleton.h"

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
	IntPtr SpineAnimationEvent::getName()
	{
		return (IntPtr)_name.c_str();
	}

	float SpineAnimationEvent::getTime()
	{
		return _time;
	}

	SpineAnimationEvent::SpineAnimationEvent(const char* name, float time): _time(time), _name(name)
	{
	}

	SpineSkeletonAnimation::SpineSkeletonAnimation(void* animation)
	{
		_spAnimation = animation;

		spAnimation* anim = static_cast<spAnimation *>(_spAnimation);
		std::vector<spEventTimeline *> evensTimelines;
		for (int i = 0; i < anim->timelinesCount; i++)
		{
			spTimeline* t = anim->timelines[i];
			if (t->type == SP_TIMELINE_EVENT)
			{
				evensTimelines.push_back(reinterpret_cast<spEventTimeline *>(t));
			}
		}

		for (int i = 0; i < evensTimelines.size(); i++)
		{
			spEventTimeline *events = evensTimelines[i];
			for (int j = 0; j < events->framesCount; j++)
			{				
				_events.push_back(new SpineAnimationEvent(events->events[j]->data->name, events->events[j]->time));
			}
		}
	}

	SpineSkeletonAnimation::~SpineSkeletonAnimation()
	{
		for (int i = 0; i < _events.size(); i++)
		{
			delete _events[i];
		}
	}

	void* SpineSkeletonAnimation::getAnimation()
	{
		return _spAnimation;
	}

	std::string SpineSkeletonAnimation::name() const
	{
		return static_cast<spAnimation *>(_spAnimation)->name;
	}

	IntPtr API_CALL SpineSkeletonAnimation::getNativeInstance()
	{
		return this;
	}

	IntPtr API_CALL SpineSkeletonAnimation::getName()
	{
		return (IntPtr)static_cast<spAnimation *>(_spAnimation)->name;
	}

	float API_CALL SpineSkeletonAnimation::getDuration()
	{
		return static_cast<spAnimation *>(_spAnimation)->duration;
	}

	llge::ISpineAnimationEvent* SpineSkeletonAnimation::getEvent(int i)
	{
		return _events[i];
	}

	int SpineSkeletonAnimation::getEventsCount()
	{
		return _events.size();
	}
}
