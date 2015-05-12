#include "SpineSkeletonResource.h"
#include "SpineSkeletonAnimation.h"
#include "SpineSkeleton.h"
#include "SpineAnimationStateData.h"

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
	void API_CALL SpineSkeletonResource::load(String atlasText, String jsonText)
	{
		_spAtlas = spAtlas_create(atlasText, strlen(atlasText), "", 0);
		_spSkeletonJson = spSkeletonJson_create((spAtlas *)_spAtlas);
		_spSkeletonData = spSkeletonJson_readSkeletonData((spSkeletonJson*)_spSkeletonJson, jsonText);

		_animations.resize(((spSkeletonData*)_spSkeletonData)->animationsCount);
		for (int i = 0; i < _animations.size(); i++)
		{
			_animations[i] = new SpineSkeletonAnimation(((spSkeletonData*)_spSkeletonData)->animations[i]);
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
	}

	llge::ISpineAnimation* API_CALL SpineSkeletonResource::getSpineAnimation(int i)
	{
		return getAnimation(i);
	}

	int API_CALL SpineSkeletonResource::getSpineAnimationsCount()
	{
		return getAnimationsCount();
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
	
	llge::ISpineSkeleton* API_CALL SpineSkeletonResource::createSkeleton()
	{
		return new SpineSkeleton(this);
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