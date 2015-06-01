#include "SpineAnimationStateData.h"
#include "SpineSkeletonResource.h"
#include "SpineSkeletonAnimation.h"
#include "SpineAnimationState.h"

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
	SpineAnimationStateData::SpineAnimationStateData(SpineSkeletonResource* resource) : _spAnimationStateData(0)
	{
		initFromResource(resource);
	}
	
	SpineAnimationStateData::~SpineAnimationStateData()
	{
		cleanup();
	}
	
	void SpineAnimationStateData::setMix(SpineSkeletonAnimation *from, SpineSkeletonAnimation *to, float time)
	{
		spAnimationStateData_setMix((spAnimationStateData *)_spAnimationStateData, (spAnimation *)from->getAnimation(), (spAnimation *)to->getAnimation(), time);
	}
	
	void SpineAnimationStateData::initFromResource(SpineSkeletonResource* resource)
	{
		_spAnimationStateData = spAnimationStateData_create((spSkeletonData *)resource->getSkeletonData());
	}
	
	void SpineAnimationStateData::cleanup()
	{
		if (_spAnimationStateData)
		{
			spAnimationStateData_dispose((spAnimationStateData *)_spAnimationStateData);
			_spAnimationStateData = 0;
		}
	}
	
	void* SpineAnimationStateData::getStateData()
	{
		return _spAnimationStateData;
	}

	void API_CALL SpineAnimationStateData::setMix(llge::ISpineAnimation *from, llge::ISpineAnimation *to, float time)
	{
		setMix((SpineSkeletonAnimation *)from->getNativeInstance(), (SpineSkeletonAnimation *)to->getNativeInstance(), time);
	}

	void API_CALL SpineAnimationStateData::dispose()
	{
		delete this;
	}

	llge::ISpineAnimationState* API_CALL SpineAnimationStateData::createState()
	{
		return new SpineAnimationState(this);
	}

}