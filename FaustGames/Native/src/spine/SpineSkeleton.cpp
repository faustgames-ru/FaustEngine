#include "SpineSkeleton.h"
#include "SpineSkeletonResource.h"

#include "..\drawing\Batcher.h"

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
	drawing::BatcherSpineMesh SpineSkeleton::_mesh(4096);
	int SpineSkeleton::_quadIndices[6] = { 0, 1, 2, 0, 2, 3 };
	float SpineSkeleton::_uvBuffer[4096];



	SpineSkeleton::SpineSkeleton(SpineSkeletonResource *resource) : _spSkeleton(0)
	{
		initFromResource(resource);
		spSkeleton *s = (spSkeleton*)_spSkeleton;
		spSkeletonData *sd = (spSkeletonData*)resource->getSkeletonData();
		if (sd->skinsCount > 1)
		{
			spSkeleton_setSkin(s, sd->skins[1]);
		}
		spSkeleton_setBonesToSetupPose(s);
		spSkeleton_setSlotsToSetupPose(s);
	}
	
	SpineSkeleton::~SpineSkeleton()
	{
		cleanup();
	}

	void SpineSkeleton::initFromResource(SpineSkeletonResource *resource)
	{
		_spSkeleton = spSkeleton_create((spSkeletonData *)resource->getSkeletonData());
	}
	
	inline uint getTextureId(void *attachmentRendererObject)
	{
		if (attachmentRendererObject)
		{
			spAtlasRegion * aRegion = (spAtlasRegion *)attachmentRendererObject;
			graphics::TextureImage2d * image = (graphics::TextureImage2d *)aRegion->page->rendererObject;
			return image->getId();
		}
		else
		{
			return 0;
		}
	}

	void spMeshAttachment_updateUVs_fixed(spMeshAttachment* self, float *uvs) {
		int i;
		float width = self->regionU2 - self->regionU, height = self->regionV2 - self->regionV;

		if (self->regionRotate) {
			for (i = 0; i < self->verticesCount; i += 2) {
				uvs[i] = self->regionU + self->regionUVs[i + 1] * width;
				uvs[i + 1] = self->regionV + height - self->regionUVs[i] * height;
			}
		}
		else {
			for (i = 0; i < self->verticesCount; i += 2) {
				uvs[i] = self->regionU + self->regionUVs[i] * width;
				uvs[i + 1] = self->regionV + self->regionUVs[i + 1] * height;
			}
		}
	}

	void spSkinnedMeshAttachment_updateUVs_fixed(spSkinnedMeshAttachment* self, float *uvs) {
		int i;
		float width = self->regionU2 - self->regionU, height = self->regionV2 - self->regionV;
		if (self->regionRotate) {
			for (i = 0; i < self->uvsCount; i += 2) {
				uvs[i] = self->regionU + self->regionUVs[i + 1] * width;
				uvs[i + 1] = self->regionV + height - self->regionUVs[i] * height;
			}
		}
		else {
			for (i = 0; i < self->uvsCount; i += 2) {
				uvs[i] = self->regionU + self->regionUVs[i] * width;
				uvs[i + 1] = self->regionV + self->regionUVs[i + 1] * height;
			}
		}
	}

	void API_CALL SpineSkeleton::render(llge::IBatch2d * batch)
	{
		drawing::Batcher* batcher = (drawing::Batcher*)batch->getNativeInstance();
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->attachment) continue;
			_mesh.Color = graphics::Color::fromRgba(slot->r*s->r, slot->g*s->g, slot->b*s->b, slot->a*s->a);
			_mesh.State.Blend = slot->data->additiveBlending == 0 
				? graphics::BlendState::Alpha 
				: graphics::BlendState::Additive;
			_mesh.State.Effect = graphics::Effects::textureColor();
			_mesh.State.LightmapId = 0;			
			_mesh.Z = 0.5f;

			switch (slot->attachment->type)
			{
				case SP_ATTACHMENT_REGION:
				{					
					spRegionAttachment * region = SUB_CAST(spRegionAttachment, slot->attachment);
					spRegionAttachment_computeWorldVertices(region, slot->bone, _mesh.Vertices);
					_mesh.Uvs = region->uvs;
					_mesh.Indices = _quadIndices;
					_mesh.IndicesCount = 6;
					_mesh.VerticesCount = 4;
					_mesh.State.TextureId = getTextureId(region->rendererObject);
					batcher->drawSpineMesh(_mesh);					
					break;
				}
				case SP_ATTACHMENT_BOUNDING_BOX:
				{
					//spBoundingBoxAttachment * boundingBox = SUB_CAST(spBoundingBoxAttachment, slot->attachment);
					//spBoundingBoxAttachment_computeWorldVertices(boundingBox, slot->bone, _mesh.Vertices);
					break;
				}
				case SP_ATTACHMENT_MESH:
				{					
					spMeshAttachment * mesh = SUB_CAST(spMeshAttachment, slot->attachment);
					spMeshAttachment_updateUVs_fixed(mesh, _uvBuffer);
					spMeshAttachment_computeWorldVertices(mesh, slot, _mesh.Vertices);
					_mesh.Uvs = _uvBuffer;
					_mesh.Indices = mesh->triangles;
					_mesh.IndicesCount = mesh->trianglesCount;
					_mesh.VerticesCount = mesh->verticesCount / 2;
					_mesh.State.TextureId = getTextureId(mesh->rendererObject);
					batcher->drawSpineMesh(_mesh);					
					break;
				}
				case SP_ATTACHMENT_SKINNED_MESH:
				{			
					
					spSkinnedMeshAttachment * skinnedMesh = SUB_CAST(spSkinnedMeshAttachment, slot->attachment);
					spSkinnedMeshAttachment_updateUVs_fixed(skinnedMesh, _uvBuffer);
					spSkinnedMeshAttachment_computeWorldVertices(skinnedMesh, slot, _mesh.Vertices);
					_mesh.Uvs = _uvBuffer;
					_mesh.Indices = skinnedMesh->triangles;
					_mesh.IndicesCount = skinnedMesh->trianglesCount;
					_mesh.VerticesCount = skinnedMesh->uvsCount / 2;
					_mesh.State.TextureId = getTextureId(skinnedMesh->rendererObject);
					batcher->drawSpineMesh(_mesh);
					
					break;
				}
				default:
				{
					break;
				}
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