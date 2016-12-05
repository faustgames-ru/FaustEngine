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

namespace spine
{
	drawing::BatcherSpineMesh SpineSkeleton::_mesh(65536);
	ushort SpineSkeleton::_quadIndices[6] = { 0, 1, 2, 0, 2, 3 };
	float SpineSkeleton::_uvBuffer[65536];

	void spMeshAttachment_updateUVs_fixed(spMeshAttachment* self, float *uvs) {
		int i;
		float width = self->regionU2 - self->regionU, height = self->regionV2 - self->regionV;

		if (self->regionRotate) {
			for (i = 0; i < self->super.worldVerticesLength; i += 2) {
				uvs[i] = self->regionU + self->regionUVs[i + 1] * width;
				uvs[i + 1] = self->regionV + height - self->regionUVs[i] * height;
			}
		}
		else {
			for (i = 0; i < self->super.worldVerticesLength; i += 2) {
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

	inline graphics::TextureImage2d * getTexture(void *attachmentRendererObject)
	{
		if (attachmentRendererObject)
		{
			spAtlasRegion * aRegion = (spAtlasRegion *)attachmentRendererObject;
			graphics::TextureImage2d * image = (graphics::TextureImage2d *)aRegion->page->rendererObject;
			return image;
		}
		else
		{
			return nullptr;
		}
	}

	SpineSkeletonSlot::SpineSkeletonSlot(void* slot)
	{
		_spSlot = slot;
	}

	void SpineSkeletonSlot::setRgb(uint color)
	{
		spSlot* slot = static_cast<spSlot*>(_spSlot);
		slot->r = graphics::Color::getRf(color);
		slot->g = graphics::Color::getGf(color);
		slot->b = graphics::Color::getBf(color);
	}

	const char* SpineSkeletonSlot::name()
	{
		return static_cast<spSlot *>(_spSlot)->data->name;
	}

	SpineSkeletonBone::SpineSkeletonBone(void* bone)
	{
		fx = llge::BoneFxNone;
		rgbTransfomrIndex = -1;
		_spBone = bone;
	}

	const char* SpineSkeletonBone::name()
	{
		return static_cast<spBone *>(_spBone)->data->name;
	}

	IntPtr SpineSkeletonBone::getName()
	{
		return (IntPtr)static_cast<spBone *>(_spBone)->data->name;
	}

	float SpineSkeletonBone::getX()
	{
		return static_cast<spBone *>(_spBone)->worldX;
	}

	float SpineSkeletonBone::getY()
	{
		return static_cast<spBone *>(_spBone)->worldY;
	}

	void SpineSkeletonBone::setBoneFx(llge::BoneFx value)
	{
		fx = value;
	}

	void SpineSkeletonBone::setBoneRgbTransfomrIndex(int index)
	{
		rgbTransfomrIndex = index;
	}

	inline bool ends_with(std::string const & value, std::string const & ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}



	SpineSkeleton::SpineSkeleton(SpineSkeletonResource *resource, float* transform) : _spSkeleton(0)
	{
		_defaultRgbTransformIndex = -1;		
		for (uint i = 0; i < _colorTransformCount; i++)
		{
			_colorTransform[i] = core::Matrix3::identity;
		}
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
		SpineSkeleton::updateWorldTransform();
		updateAabb();

		int i;
		for (i = 0; i < s->bonesCount; ++i)
		{
			SpineSkeletonBone* bone = new SpineSkeletonBone(s->bones[i]);
			std::string boneName = bone->name();
			for (int j = 0; j < _colorTransformCount; j++)
			{
				if (ends_with(boneName, _colorTransformNames[j]))
				{
					bone->setBoneRgbTransfomrIndex(j);
				}
			}
			_bones.push_back(bone);
		}
		for (i = 0; i < s->slotsCount; ++i)
		{
			SpineSkeletonSlot* slot = new SpineSkeletonSlot(s->slots[i]);
			_slots.push_back(slot);
		}
	}

	int SpineSkeleton::getBonesCount()
	{
		return _bones.size();
	}

	void SpineSkeleton::renderEx(llge::IBatch2d* batch, IntPtr effectConfig, llge::GraphicsEffects effect, byte colorScale)
	{
		_bounds.clear();
		drawing::Batcher* batcher = (drawing::Batcher*)batch->getNativeInstance();
		spSkeleton *s = (spSkeleton *)_spSkeleton;
		geometry::Aabb2d aabb;
		_mesh.Z = _transform.getWz();
		graphics::EffectBase * effectInstance = graphics::RenderConverter::getInstance()->getEffect(effect);
		graphics::EffectBase * effectInstanceRgbTransform = graphics::RenderConverter::getInstance()->getRgbTransformeffect(effect);
		
		effectInstance->configCopy(&_lightingConfig, effectConfig);
		_mesh.State.config = &_lightingConfig;
		uint lightmap = _lightingConfig.lightmap;
		bool premul = true;
		for (int i = 0; i < s->slotsCount; i++)
		{
			spSlot* slot = s->drawOrder[i];
			if (!slot->data) 
				continue;
			if (!slot->attachment) continue;
			premul = true;
			_mesh.Color = graphics::Color::fromRgba(slot->r*s->r, slot->g*s->g, slot->b*s->b, slot->a*s->a);
			_mesh.State.Blend = slot->data->blendMode == SP_BLEND_MODE_NORMAL
				? graphics::BlendState::Alpha
				: graphics::BlendState::Additive;
			
			SpineSkeletonBone* bone = _bones[slot->bone->data->index];
			
			if (_defaultRgbTransformIndex >= 0 && bone->rgbTransfomrIndex < 0)
			{
				batcher->addColorTransform(_colorTransform[_defaultRgbTransformIndex]);
				_mesh.State.Effect = effectInstanceRgbTransform;
			}
			else if (bone->rgbTransfomrIndex >= 0)
			{
				batcher->addColorTransform(_colorTransform[bone->rgbTransfomrIndex]);
				_mesh.State.Effect = effectInstanceRgbTransform;
			}
			else if (bone->fx == llge::BoneFx::BoneFxIgnoreLight)
			{
				_mesh.State.Effect = graphics::Effects::textureColor();
				_lightingConfig.lightmap = lightmap;
			}
			else if (bone->fx == llge::BoneFx::BoneFxBlur)
			{
				continue;
			}
			else
			{
				_mesh.State.Effect = effectInstance;// graphics::Effects::textureLightmapColor();
				_lightingConfig.lightmap = lightmap;
			}

			switch (slot->attachment->type)
			{
				case SP_ATTACHMENT_REGION:
				{
					spRegionAttachment* region = SUB_CAST(spRegionAttachment, slot->attachment);
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
					_mesh.texture = getTexture(region->rendererObject);
					_lightingConfig.texture = getTextureId(region->rendererObject);
					batcher->drawSpineMesh(_mesh, colorScale, premul);
					break;
				}
				case SP_ATTACHMENT_BOUNDING_BOX:
				{
					spBoundingBoxAttachment* boundingBox = SUB_CAST(spBoundingBoxAttachment, slot->attachment);
					_bounds.push_back(SpineSkeletonBounds());
					SpineSkeletonBounds& bounds = _bounds.back();
					spBoundingBoxAttachment_computeWorldVertices(boundingBox, slot, bounds.vertices);
					bounds.count = boundingBox->super.worldVerticesLength;
					break;
				}
				case SP_ATTACHMENT_LINKED_MESH:
				{
					break;
				}
				case SP_ATTACHMENT_MESH:
				{
					spMeshAttachment* mesh = SUB_CAST(spMeshAttachment, slot->attachment);
					spMeshAttachment_computeWorldVertices(mesh, slot, _mesh.Vertices);

					spMeshAttachment_updateUVs_fixed(mesh, _uvBuffer);

					for (int j = 0; j < mesh->super.worldVerticesLength; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					_mesh.Uvs = _uvBuffer;
					_mesh.Indices = mesh->triangles;
					_mesh.IndicesCount = mesh->trianglesCount;
					_mesh.VerticesCount = mesh->super.worldVerticesLength / 2;
					_mesh.texture = getTexture(mesh->rendererObject);
					_lightingConfig.texture = getTextureId(mesh->rendererObject);
					batcher->drawSpineMesh(_mesh, colorScale, premul);
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

	float SpineSkeleton::getBoundsVertexX(int boundsIndex, int vertexIndex)
	{
		return _bounds[boundsIndex].vertices[vertexIndex*2+0];
	}

	float SpineSkeleton::getBoundsVertexY(int boundsIndex, int vertexIndex)
	{
		return _bounds[boundsIndex].vertices[vertexIndex*2 + 1];
	}

	int SpineSkeleton::getBoundsVerticesCount(int boundsIndex)
	{
		return _bounds[boundsIndex].count / 2;
	}

	int SpineSkeleton::getBoundsCount()
	{
		return _bounds.size();
	}

	llge::ISpineSkeletonBone* SpineSkeleton::getBone(int index)
	{
		return _bones[index];
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
		_bounds.clear();
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
					spBoundingBoxAttachment * boundingBox = SUB_CAST(spBoundingBoxAttachment, slot->attachment);
					_bounds.push_back(SpineSkeletonBounds());
					SpineSkeletonBounds& bounds = _bounds.back();
					spBoundingBoxAttachment_computeWorldVertices(boundingBox, slot, bounds.vertices);
					bounds.count = boundingBox->super.worldVerticesLength;
					break;
				}
				case SP_ATTACHMENT_LINKED_MESH:
				case SP_ATTACHMENT_MESH:
				{
					spMeshAttachment * mesh = SUB_CAST(spMeshAttachment, slot->attachment);
					spMeshAttachment_computeWorldVertices(mesh, slot, _mesh.Vertices);
					for (int j = 0; j < mesh->super.worldVerticesLength; j += 2)
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
					_mesh.texture = getTexture(region->rendererObject);
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
					for (int j = 0; j < mesh->super.worldVerticesLength; j += 2)
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
					_mesh.Indices = mesh->triangles;
					_mesh.IndicesCount = mesh->trianglesCount;
					_mesh.texture = getTexture(mesh->rendererObject);
					_mesh.VerticesCount = mesh->super.worldVerticesLength / 2;
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



	void API_CALL SpineSkeleton::render(llge::IBatch2d * batch, int lightmapId, llge::GraphicsEffects effect, byte colorScale)
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
			_mesh.State.Blend = slot->data->blendMode == SP_BLEND_MODE_NORMAL
				? graphics::BlendState::Alpha 
				: graphics::BlendState::Additive;
			
			SpineSkeletonBone* bone = _bones[slot->bone->data->index];
			if (bone->fx == llge::BoneFx::BoneFxIgnoreLight)
			{
				_mesh.State.Effect = graphics::Effects::textureColor();
			}
			else if (bone->fx == llge::BoneFx::BoneFxBlur)
			{
				continue;
			}
			else
			{
				_mesh.State.Effect = effectInstance;
			}

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
					_mesh.texture = getTexture(region->rendererObject);
					//_mesh.State.TextureId = getTextureId(region->rendererObject);
					_lightingConfig.texture = getTextureId(region->rendererObject);
					batcher->drawSpineMesh(_mesh, colorScale, false);
					break;
				}
				case SP_ATTACHMENT_BOUNDING_BOX:
				{
					//spBoundingBoxAttachment * boundingBox = SUB_CAST(spBoundingBoxAttachment, slot->attachment);
					//spBoundingBoxAttachment_computeWorldVertices(boundingBox, slot->bone, _mesh.Vertices);
					break;
				}
				case SP_ATTACHMENT_LINKED_MESH:
				case SP_ATTACHMENT_MESH:
				{					
					spMeshAttachment * mesh = SUB_CAST(spMeshAttachment, slot->attachment);
					spMeshAttachment_updateUVs_fixed(mesh, _uvBuffer);
					spMeshAttachment_computeWorldVertices(mesh, slot, _mesh.Vertices);
					for (int j = 0; j < mesh->super.worldVerticesLength; j += 2)
					{
						transform(_mesh.Vertices + j, _mesh.Vertices + j + 1);
						aabb.expand(_mesh.Vertices[j], _mesh.Vertices[j + 1]);
					}
					_mesh.Uvs = _uvBuffer;
					_mesh.Indices = mesh->triangles;
					_mesh.IndicesCount = mesh->trianglesCount;
					_mesh.VerticesCount = mesh->super.worldVerticesLength / 2;
					_mesh.texture = getTexture(mesh->rendererObject);
					//_mesh.State.TextureId = getTextureId(mesh->rendererObject);
					_lightingConfig.texture = getTextureId(mesh->rendererObject);
					batcher->drawSpineMesh(_mesh, colorScale, false);
					break;
				}
				/*
				case SP_ATTACHMENT_WEIGHTED_MESH:
				{			
					
					spWeightedMeshAttachment * skinnedMesh = SUB_CAST(spWeightedMeshAttachment, slot->attachment);
					spSkinnedMeshAttachment_updateUVs_fixed(skinnedMesh, _uvBuffer);
					spWeightedMeshAttachment_computeWorldVertices(skinnedMesh, slot, _mesh.Vertices);
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
					batcher->drawSpineMesh(_mesh, colorScale);
					break;
				}
				*/
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
			for (uint i = 0; i < _bones.size(); i++)
			{
				delete _bones[i];
			}
			spSkeleton_dispose(static_cast<spSkeleton *>(_spSkeleton));
			_spSkeleton = 0;
		}
	}

	void API_CALL SpineSkeleton::updateWorldTransform()
	{
		spSkeleton_updateWorldTransform(static_cast<spSkeleton *>(_spSkeleton));
	}


	void* SpineSkeleton::getSkeleton() const
	{
		return _spSkeleton;
	}

	
	SpineSkeletonBone* SpineSkeleton::findBone(const char* boneName)
	{
		for (uint i = 0; i < _bones.size(); i++)
		{
			if (std::string(_bones[i]->name()) == boneName)
			{
				return _bones[i];
			}
		}
		return nullptr;
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

	void SpineSkeleton::setRgbTransform(int index, void* floatMatrix3)
	{
		_colorTransform[index].setData(static_cast<float *>(floatMatrix3));
        _colorTransform[index] = _colorTransform[index].transpose();
	}

	SpineSkeletonSlot* SpineSkeleton::findSlot(const char* slotName)
	{
		for (uint i = 0; i < _slots.size(); i++)
		{
			if (std::string(_slots[i]->name()) == slotName)
			{
				return _slots[i];
			}
		}
		return nullptr;
	}

	void SpineSkeleton::setSlotRgb(const char* name, int color)
	{
		spine::SpineSkeletonSlot* slot = findSlot(name);
		if (name != nullptr)
		{
			slot->setRgb(color);
		}
	}

	void SpineSkeleton::setDefaultRgbTransform(int index)
	{
		_defaultRgbTransformIndex = index;
	}

	std::string SpineSkeleton::_colorTransformNames[16]
	{
		"tint00",
		"tint01",
		"tint02",
		"tint03",
		"tint04",
		"tint05",
		"tint06",
		"tint07",
		"tint08",
		"tint09",
		"tint10",
		"tint11",
		"tint12",
		"tint13",
		"tint14",
		"tint15",
	};
}
