#include "SpineSkeletonResource.h"
#include "SpineSkeletonAnimation.h"
#include "SpineSkeleton.h"
#include "SpineAnimationStateData.h"

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
	IntPtr API_CALL SpineEvent::getName()
	{
		return (IntPtr)_name.c_str();
	}

	SpineEvent::SpineEvent(const char * name)
	{
		_name = name;
	}

	IntPtr API_CALL SpineSkin::getName() 
	{
		return (IntPtr)_name.c_str();
	}

	IntPtr API_CALL SpineSkin::getNativeInstance() 
	{
		return _spSkin;
	}
	SpineSkin::SpineSkin(void * skin) 
	{
		_spSkin = skin;
		spSkin* s = (spSkin*)_spSkin;
		_name = s->name;
	}



	void API_CALL SpineSkeletonResource::load(String atlasText, String jsonText, String dir)
	{
		_spAtlas = spAtlas_create(atlasText, strlen(atlasText), dir, 0);
		_spSkeletonJson = spSkeletonJson_create((spAtlas *)_spAtlas);
		_spSkeletonData = spSkeletonJson_readSkeletonData((spSkeletonJson*)_spSkeletonJson, jsonText);
		

		spSkeletonData* sd = (spSkeletonData*)_spSkeletonData;
		_animations.resize(sd->animationsCount);
		
		for (int i = 0; i < _animations.size(); i++)
		{
        	_animations[i] = new SpineSkeletonAnimation(((spSkeletonData*)_spSkeletonData)->animations[i]);
		}

		_events.resize(sd->eventsCount);
		for (int i = 0; i < _events.size(); i++)
		{
			spEventData* e = sd->events[i];
			e->intValue = i;
			_events[i] = new SpineEvent(e->name);
		}
		_skins.resize(sd->skinsCount);
		for (int i = 0; i < _skins.size(); i++)
		{
			spSkin* s = sd->skins[i];
			_skins[i] = new SpineSkin(s);
		}
	}

	void API_CALL SpineSkeletonResource::unLoad()
	{
		spSkeletonData_dispose((spSkeletonData *)_spSkeletonData);
		_spSkeletonData = 0;
		
		spSkeletonJson_dispose((spSkeletonJson *)_spSkeletonJson);
		_spSkeletonJson = 0;

		spAtlas_dispose((spAtlas *)_spAtlas);
		_spAtlas = 0;

		for (int i = 0; i < _animations.size(); i++)
		{
			delete _animations[i];
		}
		_animations.clear();
		for (int i = 0; i < _events.size(); i++)
		{
			delete _events[i];
		}
		_events.clear();
		for (int i = 0; i < _skins.size(); i++)
		{
			delete _skins[i];
		}
		_skins.clear();
	}

	llge::ISpineAnimation* API_CALL SpineSkeletonResource::getSpineAnimation(int i)
	{
		return getAnimation(i);
	}
	
	int API_CALL SpineSkeletonResource::getSpineAnimationsCount()
	{
		return getAnimationsCount();
	}

	llge::ISpineSkin* API_CALL SpineSkeletonResource::getSpineSkin(int i) 
	{
		return _skins[i];
	}

	int API_CALL SpineSkeletonResource::getSpineSkinsCount() 
	{
		return _skins.size();
	}

	llge::ISpineEvent* API_CALL SpineSkeletonResource::getSpineEvent(int i)
	{
		return getEvent(i);
	}

	int API_CALL SpineSkeletonResource::getSpineEventsCount()
	{
		return getEventsCount();
	}


	void* SpineSkeletonResource::getSkeletonData()
	{
		return _spSkeletonData;
	}

	SpineSkeletonAnimation* SpineSkeletonResource::getAnimation(int i)
	{
		return _animations[i];
	}
	
	int SpineSkeletonResource::getAnimationsCount()
	{
		return _animations.size();
	}

	SpineEvent* SpineSkeletonResource::getEvent(int i)
	{
		return _events[i];
	}

	int SpineSkeletonResource::getEventsCount()
	{
		return _events.size();
	}
	
	llge::ISpineSkeleton* API_CALL SpineSkeletonResource::createSkeleton(void *floatMatrix)
	{
		return new SpineSkeleton(this, (float *)floatMatrix);
	}
	
	llge::ISpineAnimationStateData* API_CALL SpineSkeletonResource::createStateData()
	{
		return new SpineAnimationStateData(this);
	}

	void API_CALL SpineSkeletonResource::dispose()
	{
		delete this;
	}
}