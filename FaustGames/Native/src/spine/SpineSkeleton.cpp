#include "SpineSkeleton.h"
#include "SpineSkeletonResource.h"

#include "spine\extension.h"
#include "spine\Skeleton.h"
#include "spine\Animation.h"
#include "spine\AnimationState.h"
#include "spine\AnimationStateData.h"
#include "spine\SkeletonJson.h"
#include "spine\SkeletonData.h"
#include "spine\Atlas.h"
#include "spine\AtlasAttachmentLoader.h"
#include "spine\Attachment.h"
#include "spine\RegionAttachment.h"
#include "spine\BoundingBoxAttachment.h"
#include "spine\MeshAttachment.h"
#include "spine\SkinnedMeshAttachment.h"

namespace spine
{
	SpineSkeleton::SpineSkeleton(SpineSkeletonResource *resource) : _spSkeleton(0)
	{
		initFromResource(resource);
	}
	
	SpineSkeleton::~SpineSkeleton()
	{
		cleanup();
	}

	void SpineSkeleton::initFromResource(SpineSkeletonResource *resource)
	{
		_spSkeleton = spSkeleton_create((spSkeletonData *)resource->getSkeletonData());
	}
	
	void API_CALL SpineSkeleton::render(llge::IBatch2d * batch)
	{
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		float *worldCoords = 0;
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->attachment) continue;
			switch (slot->attachment->type)
			{
			case SP_ATTACHMENT_REGION:		
				spRegionAttachment * region = SUB_CAST(spRegionAttachment, slot->attachment);
				spRegionAttachment_computeWorldVertices(region, slot->bone, worldCoords);
				break;
			case SP_ATTACHMENT_BOUNDING_BOX:
				spBoundingBoxAttachment * boundingBox = SUB_CAST(spBoundingBoxAttachment, slot->attachment);
				spBoundingBoxAttachment_computeWorldVertices(boundingBox, slot->bone, worldCoords);
				break;
			case SP_ATTACHMENT_MESH:
				spMeshAttachment * mesh = SUB_CAST(spMeshAttachment, slot->attachment);
				spMeshAttachment_computeWorldVertices(mesh, slot, worldCoords);
				break;
			case SP_ATTACHMENT_SKINNED_MESH:
				spSkinnedMeshAttachment * skinnedMesh = SUB_CAST(spSkinnedMeshAttachment, slot->attachment);
				spSkinnedMeshAttachment_computeWorldVertices(skinnedMesh, slot, worldCoords);
				break;
			default:
				break;
			}
		}
	}


	void SpineSkeleton::cleanup()
	{
		if (_spSkeleton)
		{
			spSkeleton_dispose((spSkeleton *)_spSkeleton);
			_spSkeleton = 0;
		}
	}

	void API_CALL SpineSkeleton::updateWorldTransform()
	{
		spSkeleton_updateWorldTransform((spSkeleton *)_spSkeleton);
	}


	void* SpineSkeleton::getSkeleton()
	{
		return _spSkeleton;
	}

	IntPtr API_CALL SpineSkeleton::getNativeInstance()
	{
		return this;
	}

	void API_CALL SpineSkeleton::dispose()
	{
		delete this;
	}
}