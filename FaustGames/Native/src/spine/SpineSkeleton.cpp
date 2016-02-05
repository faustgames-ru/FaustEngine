#include "SpineSkeleton.h"
#include "SpineSkeletonResource.h"

#include "../drawing/Batcher.h"

#include "spine/extension.h"
#include "spine/Skeleton.h"
#include "spine/Animation.h"
#include "spine/AnimationState.h"
#include "spine/AnimationStateData.h"
#include "spine/SkeletonJson.h"
#include "spine/SkeletonData.h"
#include "spine/Atlas.h"
#include "spine/AtlasAttachmentLoader.h"
#include "spine/Attachment.h"
#include "spine/RegionAttachment.h"
#include "spine/BoundingBoxAttachment.h"
#include "spine/MeshAttachment.h"
#include "spine/SkinnedMeshAttachment.h"

namespace spine
{
	drawing::BatcherSpineMesh SpineSkeleton::_mesh(4096);
	int SpineSkeleton::_quadIndices[6] = { 0, 1, 2, 0, 2, 3 };
	float SpineSkeleton::_uvBuffer[4096];

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

	SpineSkeleton::SpineSkeleton(SpineSkeletonResource *resource, float* transform) : _spSkeleton(0)
	{
		initFromResource(resource);
		spSkeleton *s = (spSkeleton*)_spSkeleton;
		spSkeletonData *sd = (spSkeletonData*)resource->getSkeletonData();
		if (sd->skinsCount > 1)
		{
			spSkeleton_setSkin(s, sd->skins[1]);
		}
		if (transform)
			_transform.setData(transform);
		else
			_transform = core::Matrix::identity;
		spSkeleton_setToSetupPose(s);
		updateWorldTransform();
		updateAabb();
	}

	void SpineSkeleton::renderEx(llge::IBatch2d* batch, IntPtr effectConfig, llge::GraphicsEffects effect)
	{
		drawing::Batcher* batcher = (drawing::Batcher*)batch->getNativeInstance();
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		geometry::Aabb2d aabb;
		_mesh.Z = _transform.getWz();
		graphics::EffectBase * effectInstance = graphics::RenderConverter::getInstance()->getEffect(effect);
		effectInstance->configCopy(&_lightingConfig, effectConfig);
		_mesh.State.config = &_lightingConfig;
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->attachment) continue;
			_mesh.Color = graphics::Color::fromRgba(slot->r*s->r, slot->g*s->g, slot->b*s->b, slot->a*s->a);
			_mesh.State.Blend = slot->data->additiveBlending == 0
				? graphics::BlendState::Alpha
				: graphics::BlendState::Additive;

			_mesh.State.Effect = effectInstance;// graphics::Effects::textureLightmapColor();

			switch (slot->attachment->type)
			{
			case SP_ATTACHMENT_REGION:
			{
				spRegionAttachment * region = SUB_CAST(spRegionAttachment, slot->attachment);
				spRegionAttachment_computeWorldVertices(region, slot->bone, _mesh.Vertices);
				for (int j = 0; j < 8; j += 2)
				{
					transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
				}
				_mesh.Uvs = region->uvs;
				_mesh.Indices = _quadIndices;
				_mesh.IndicesCount = 6;
				_mesh.VerticesCount = 4;
				//_mesh.State.TextureId = getTextureId(region->rendererObject);
				_lightingConfig.texture = getTextureId(region->rendererObject);
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
				for (int j = 0; j < mesh->verticesCount; j += 2)
				{
					transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
				}
				_mesh.Uvs = _uvBuffer;
				_mesh.Indices = mesh->triangles;
				_mesh.IndicesCount = mesh->trianglesCount;
				_mesh.VerticesCount = mesh->verticesCount / 2;
				//_mesh.State.TextureId = getTextureId(mesh->rendererObject);
				_lightingConfig.texture = getTextureId(mesh->rendererObject);
				batcher->drawSpineMesh(_mesh);
				break;
			}
			case SP_ATTACHMENT_SKINNED_MESH:
			{

				spSkinnedMeshAttachment * skinnedMesh = SUB_CAST(spSkinnedMeshAttachment, slot->attachment);
				spSkinnedMeshAttachment_updateUVs_fixed(skinnedMesh, _uvBuffer);
				spSkinnedMeshAttachment_computeWorldVertices(skinnedMesh, slot, _mesh.Vertices);
				for (int j = 0; j < skinnedMesh->uvsCount; j += 2)
				{
					transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
				}
				_mesh.Uvs = _uvBuffer;
				_mesh.Indices = skinnedMesh->triangles;
				_mesh.IndicesCount = skinnedMesh->trianglesCount;
				_mesh.VerticesCount = skinnedMesh->uvsCount / 2;
				_lightingConfig.texture = getTextureId(skinnedMesh->rendererObject);
				//_mesh.State.TextureId = getTextureId(skinnedMesh->rendererObject);
				batcher->drawSpineMesh(_mesh);
				break;
			}
			default:
			{
				break;
			}
			}
		}
		_aabb = aabb;
	}

	SpineSkeleton::~SpineSkeleton()
	{
		cleanup();
	}

	void API_CALL SpineSkeleton::applySkin(IntPtr spineSkinNativeInstance)
	{
		spSkeleton *s = (spSkeleton*)_spSkeleton;
		spSkeleton_setSkin(s, (spSkin *)spineSkinNativeInstance);
	}
	
	void SpineSkeleton::transform(float *x, float *y)
	{
		float x0 = *x;
		float y0 = *y;
		*x = x0 * _transform.getXx() + y0 * _transform.getYx() + _transform.getWx();
		*y = x0 * _transform.getXy() + y0 * _transform.getYy() + _transform.getWy();
	}

	void SpineSkeleton::updateAabb()
	{		
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		geometry::Aabb2d aabb;
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->attachment) continue;
			switch (slot->attachment->type)
			{
				case SP_ATTACHMENT_REGION:
				{
					spRegionAttachment * region = SUB_CAST(spRegionAttachment, slot->attachment);
					spRegionAttachment_computeWorldVertices(region, slot->bone, _mesh.Vertices);
					for (int j = 0; j < 8; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
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
					spMeshAttachment_computeWorldVertices(mesh, slot, _mesh.Vertices);
					for (int j = 0; j < mesh->verticesCount; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					break;
				}
				case SP_ATTACHMENT_SKINNED_MESH:
				{
					spSkinnedMeshAttachment * skinnedMesh = SUB_CAST(spSkinnedMeshAttachment, slot->attachment);
					spSkinnedMeshAttachment_computeWorldVertices(skinnedMesh, slot, _mesh.Vertices);
					for (int j = 0; j < skinnedMesh->uvsCount; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
		_aabb = aabb;
	}

	void SpineSkeleton::initFromResource(SpineSkeletonResource *resource)
	{
		_spSkeleton = spSkeleton_create((spSkeletonData *)resource->getSkeletonData());
	}
		
	class VBuffer
	{
	public:
		drawing::Mesh2dVertex *Vertices;
		ushort *Indices;
		int VerticesLimit;
		int IndicesLimit;
		VBuffer(drawing::Mesh2dVertex *vertices, int verticeLimit, ushort *indices, int indicesLimit)
			: Vertices(vertices), Indices(indices), VerticesLimit(verticeLimit), IndicesLimit(indicesLimit), _verticesCount(0), _indicesCount(0)
		{
		}
		void Add(const drawing::BatcherSpineMesh &mesh)
		{
			if ((_indicesCount + mesh.IndicesCount) > IndicesLimit) return;
			if ((_verticesCount + mesh.VerticesCount) > VerticesLimit) return;
			for (int i = 0; i < mesh.IndicesCount; i++)
			{
				Indices[_indicesCount + i] = (ushort)(_verticesCount + mesh.Indices[i]);
			}

			drawing::Mesh2dVertex * target = Vertices + _verticesCount;
			float* source = mesh.Vertices;
			for (int i = 0; i < mesh.VerticesCount; i++, target++)
			{
				target->setX(*source); ++source;
				target->setY(*source); ++source;
				target->setZ(mesh.Z);
			}

			_verticesCount += mesh.VerticesCount;
			_indicesCount += mesh.IndicesCount;
		}
		inline int getIndicesCount()
		{
			return _indicesCount;
		}
	private:
		int _indicesCount;
		int _verticesCount;

	};

	int API_CALL SpineSkeleton::getGeometry(void *vertices, int verticeLimit, void *indices, int indicesLimit)
	{
		VBuffer buffer((drawing::Mesh2dVertex *)vertices, verticeLimit, (ushort *)indices, indicesLimit);
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		_mesh.Z = _transform.getWz();
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->attachment) continue;
			switch (slot->attachment->type)
			{
				case SP_ATTACHMENT_REGION:
				{
					spRegionAttachment * region = SUB_CAST(spRegionAttachment, slot->attachment);
					spRegionAttachment_computeWorldVertices(region, slot->bone, _mesh.Vertices);
					for (int j = 0; j < 8; j += 2)
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					_mesh.Indices = _quadIndices;
					_mesh.IndicesCount = 6;
					_mesh.VerticesCount = 4;
					buffer.Add(_mesh);
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
					for (int j = 0; j < mesh->verticesCount; j += 2)
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					_mesh.Indices = mesh->triangles;
					_mesh.IndicesCount = mesh->trianglesCount;
					_mesh.VerticesCount = mesh->verticesCount / 2;
					buffer.Add(_mesh);
					break;
				}
				case SP_ATTACHMENT_SKINNED_MESH:
				{

					spSkinnedMeshAttachment * skinnedMesh = SUB_CAST(spSkinnedMeshAttachment, slot->attachment);
					spSkinnedMeshAttachment_updateUVs_fixed(skinnedMesh, _uvBuffer);
					spSkinnedMeshAttachment_computeWorldVertices(skinnedMesh, slot, _mesh.Vertices);
					for (int j = 0; j < skinnedMesh->uvsCount; j += 2)
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					_mesh.Indices = skinnedMesh->triangles;
					_mesh.IndicesCount = skinnedMesh->trianglesCount;
					_mesh.VerticesCount = skinnedMesh->uvsCount / 2;
					buffer.Add(_mesh);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		return buffer.getIndicesCount();
	}



	void API_CALL SpineSkeleton::render(llge::IBatch2d * batch, int lightmapId, llge::GraphicsEffects effect)
	{
		drawing::Batcher* batcher = (drawing::Batcher*)batch->getNativeInstance();
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		geometry::Aabb2d aabb;
		_mesh.Z = _transform.getWz();
		_lightingConfig.lightmap = lightmapId;
		_mesh.State.config = &_lightingConfig;
		//_mesh.State.LightmapId = lightmapId;
		graphics::EffectBase * effectInstance = graphics::RenderConverter::getInstance()->getEffect(effect);
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->attachment) continue;
			_mesh.Color = graphics::Color::fromRgba(slot->r*s->r, slot->g*s->g, slot->b*s->b, slot->a*s->a);
			_mesh.State.Blend = slot->data->additiveBlending == 0 
				? graphics::BlendState::Alpha 
				: graphics::BlendState::Additive;

			_mesh.State.Effect = effectInstance;// graphics::Effects::textureLightmapColor();

			switch (slot->attachment->type)
			{
				case SP_ATTACHMENT_REGION:
				{					
					spRegionAttachment * region = SUB_CAST(spRegionAttachment, slot->attachment);
					spRegionAttachment_computeWorldVertices(region, slot->bone, _mesh.Vertices);
					for (int j = 0; j < 8; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					_mesh.Uvs = region->uvs;
					_mesh.Indices = _quadIndices;
					_mesh.IndicesCount = 6;
					_mesh.VerticesCount = 4;
					//_mesh.State.TextureId = getTextureId(region->rendererObject);
					_lightingConfig.texture = getTextureId(region->rendererObject);
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
					for (int j = 0; j < mesh->verticesCount; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					_mesh.Uvs = _uvBuffer;
					_mesh.Indices = mesh->triangles;
					_mesh.IndicesCount = mesh->trianglesCount;
					_mesh.VerticesCount = mesh->verticesCount / 2;
					//_mesh.State.TextureId = getTextureId(mesh->rendererObject);
					_lightingConfig.texture = getTextureId(mesh->rendererObject);
					batcher->drawSpineMesh(_mesh);
					break;
				}
				case SP_ATTACHMENT_SKINNED_MESH:
				{			
					
					spSkinnedMeshAttachment * skinnedMesh = SUB_CAST(spSkinnedMeshAttachment, slot->attachment);
					spSkinnedMeshAttachment_updateUVs_fixed(skinnedMesh, _uvBuffer);
					spSkinnedMeshAttachment_computeWorldVertices(skinnedMesh, slot, _mesh.Vertices);
					for (int j = 0; j < skinnedMesh->uvsCount; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					_mesh.Uvs = _uvBuffer;
					_mesh.Indices = skinnedMesh->triangles;
					_mesh.IndicesCount = skinnedMesh->trianglesCount;
					_mesh.VerticesCount = skinnedMesh->uvsCount / 2;
					_lightingConfig.texture = getTextureId(skinnedMesh->rendererObject);
					//_mesh.State.TextureId = getTextureId(skinnedMesh->rendererObject);
					batcher->drawSpineMesh(_mesh);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		_aabb = aabb;
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

	void API_CALL SpineSkeleton::setTransform(void *floatMatrix)
	{
		_transform.setData((float *)floatMatrix);
		updateAabb();
	}

	void API_CALL SpineSkeleton::setColor(uint color)
	{
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		s->r = graphics::Color::getRf(color);
		s->g = graphics::Color::getGf(color);
		s->b = graphics::Color::getBf(color);
		s->a = graphics::Color::getAf(color);
	}

	void API_CALL SpineSkeleton::setBonesToSetupPose()
	{
		spSkeleton_setBonesToSetupPose((spSkeleton *)_spSkeleton);
	}
	
	void API_CALL SpineSkeleton::setSlotsToSetupPose()
	{
		spSkeleton_setSlotsToSetupPose((spSkeleton *)_spSkeleton);
	}


	float API_CALL SpineSkeleton::getMinX()
	{
		return _aabb.Min.getX();
	}

	float API_CALL SpineSkeleton::getMinY()
	{
		return _aabb.Min.getY();
	}

	float API_CALL SpineSkeleton::getMaxX()
	{
		return _aabb.Max.getX();
	}

	float API_CALL SpineSkeleton::getMaxY()
	{
		return _aabb.Max.getY();
	}

	float API_CALL SpineSkeleton::getZ()
	{
		return  _transform.getWz();
	}


}