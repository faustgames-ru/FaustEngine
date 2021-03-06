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
#include <spine/extension.h>

namespace spine
{
	void spSkin_addSkin(spSkin* target, spSkin* source)
	{
		if (source == nullptr) return;
		_spSkin* src = SUB_CAST(_spSkin, source);

		const _Entry *entry = src->entries;
		while (entry) {
			spSkin_addAttachment(target, entry->slotIndex, entry->name, entry->attachment);
			entry = entry->next;
		}
	}

	void spSkin_getAllAttachmentSlot(spSkin* skin, int indexSlot, std::vector<spAttachment*>& attachments)
	{
		if (skin == nullptr) return;
		_spSkin* src = SUB_CAST(_spSkin, skin);

		const _Entry *entry = src->entries;
		while (entry) {
			if (entry->slotIndex == indexSlot)
				attachments.push_back(entry->attachment);
			entry = entry->next;
		}
	}
	
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
		spSkin* s = static_cast<spSkin*>(_spSkin);
		_name = s->name;
	}

	llge::ISpineSkin* SpineDynamicSkin::skinValue()
	{
		return _value;
	}

	void SpineDynamicSkin::addSkin(llge::ISpineSkin* value)
	{
		spSkin* src = static_cast<spSkin*>(value->getNativeInstance());
		spSkin* dst = static_cast<spSkin*>(_value->getNativeInstance());
		spSkin_addSkin(dst, src);
	}

	void spDynamicSkinEntry_Dispose(_Entry* self) {
		FREE(self->name);
		FREE(self);
	}
	
	void spDynamicSkin_dispose(spSkin* self) {
		_Entry* entry = SUB_CAST(_spSkin, self)->entries;
		while (entry) {
			_Entry* nextEntry = entry->next;
			spDynamicSkinEntry_Dispose(entry);
			entry = nextEntry;
		}

		FREE(self->name);
		FREE(self);
	}

	void SpineDynamicSkin::dispose()
	{
		spSkin* nativeSkin = static_cast<spSkin*>(_value->getNativeInstance());
		spDynamicSkin_dispose(nativeSkin);
		delete _value;
		_value = nullptr;
	}

	SpineDynamicSkin::SpineDynamicSkin()
	{
		_value = new SpineSkin(spSkin_create("dynamic_skin"));
	}

	SpineDynamicSkin::~SpineDynamicSkin()
	{
		dispose();
	}

	void API_CALL SpineSkeletonResource::load(String atlasText, String jsonText, String dir, llge::TextureQueryFormat format, float applyedCompression, llge::ITexturesManager* textures)
	{		
		loadInternal(atlasText, jsonText, dir, format, applyedCompression, nullptr, textures);
	}

	void API_CALL SpineSkeletonResource::loadWithPngImage(String atlasText, String jsonText, String dir, void* texture)
	{
		loadInternal(atlasText, jsonText, dir, llge::TQFNone, 1.0f, texture, nullptr);
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
		
		for (int i = 0; i < _dynamicSkins.size(); i++)
		{
			delete _dynamicSkins[i];
		}
		_dynamicSkins.clear();

		delete _defaultSkin;
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

	void SpineSkeletonResource::loadInternal(String atlasText, String jsonText, String dir, llge::TextureQueryFormat format, float applyedCompression, void* texture, llge::ITexturesManager* textures)
	{
		_atlasRenderObject.pagesFormat = format;
		_atlasRenderObject.applyedCompression = applyedCompression;
		_atlasRenderObject.texture = texture;
		_atlasRenderObject.texturesManager = textures;
		//spAtlas* atlas = spAtlas_create(atlasText, strlen(atlasText), dir, &_atlasRenderObject);

		_spAtlas = spAtlas_create(atlasText, strlen(atlasText), dir, &_atlasRenderObject);
		_spSkeletonJson = spSkeletonJson_create((spAtlas *)_spAtlas);
		_spSkeletonData = spSkeletonJson_readSkeletonData((spSkeletonJson*)_spSkeletonJson, jsonText);

		if (_spSkeletonData == nullptr)
		{
			_error = ((spSkeletonJson*)_spSkeletonJson)->error;
			return;
		}

		spSkeletonData* sd = (spSkeletonData*)_spSkeletonData;
		_defaultSkin = new SpineSkin(sd->defaultSkin);
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

	llge::ISpineSkeleton* API_CALL SpineSkeletonResource::createSkeleton(void *floatMatrix)
	{
		return new SpineSkeleton(this, (float *)floatMatrix);
	}
	
	llge::ISpineAnimationStateData* API_CALL SpineSkeletonResource::createStateData()
	{
		return new SpineAnimationStateData(this);
	}

	llge::ISpineDynamicSkin* SpineSkeletonResource::createDynamicSkin()
	{
		SpineDynamicSkin* dynamicSkin = new SpineDynamicSkin();
		_dynamicSkins.push_back(dynamicSkin);
		return dynamicSkin;
	}

	llge::ISpineSkin* SpineSkeletonResource::getDefaultSkin()
	{
		return _defaultSkin;
	}

	IntPtr SpineSkeletonResource::errorMessage()
	{
		return const_cast<void *>(static_cast<const void *>(_error.c_str()));
	}

	bool SpineSkeletonResource::isValid()
	{
		return _spSkeletonData != nullptr;
	}

	void API_CALL SpineSkeletonResource::dispose()
	{
		delete this;
	}

	SpineSkeletonAnimation* SpineSkeletonResource::findAnimation(const char* name)
	{
		for (uint i = 0; i < _animations.size(); i++)
		{
			if (_animations[i]->name() == name)
				return _animations[i];
		}
		return nullptr;
	}

	void SpineSkeletonResource::InitRgbTransformRegions(int slotCount, const char* rgbTransformName) const
	{
		spAtlas* atlas = (spAtlas *)_spAtlas;
		if (!atlas)
			return;
		
		std::vector<std::string> regionNames;
		
		std::map<std::string, spAtlasRegion*> mapRegions;
		int i = 0;
		spAtlasRegion* region = atlas->regions;
		while (region)
		{
			std::string name(region->page->name);
			size_t index = name.find("generated_tint");
			regionNames.push_back(index > name.size() && name.size() > 4 ? name.substr(0, name.size() - 4) : "");
			
			index = name.find(std::string(rgbTransformName) + "_generated_tint");
			if (index < name.size())
			{
				for (int j = i; j >= 0; --j)
				{
					if (regionNames[j] == "")
						continue;

					size_t res = name.find(regionNames[j]);
					if (res < name.size())
					{
						mapRegions[region->name] = region;
					}
				}
			}
			region = region->next;
			++i;
		}

		for (int skIndex = 0; skIndex < _skins.size(); ++skIndex)
		{
			spSkin* skin = static_cast<spSkin*>(_skins[skIndex]->getNativeInstance());
			
			for (int j = 0; j < slotCount; ++j)
			{
				std::vector<spAttachment*> attachments;
				spSkin_getAllAttachmentSlot(skin, j, attachments);
				if (attachments.size() <= 0)
					continue;

				for (int i = 0; i < attachments.size(); ++i)
				{
					switch (attachments[i]->type)
					{
						case SP_ATTACHMENT_REGION:
						{	
							spRegionAttachment* region = SUB_CAST(spRegionAttachment, attachments[i]);
							if (region == nullptr) continue;
							std::string attachmentName = region->path ? region->path : attachments[i]->name;
							std::map<std::string, spAtlasRegion*>::iterator regionIter = mapRegions.find(attachmentName);
							if (regionIter == mapRegions.end())
							{
								region->rgbTransformRendererObject = nullptr;
							}
							else
							{
								region->rgbTransformRendererObject = regionIter->second;
							}
						}
						break;

						case SP_ATTACHMENT_MESH:
						{
							spMeshAttachment* mesh = SUB_CAST(spMeshAttachment, attachments[i]);
							if (mesh == nullptr) continue;
							std::string attachmentName = mesh->path ? mesh->path : attachments[i]->name;
							std::map<std::string, spAtlasRegion*>::iterator regionIter = mapRegions.find(attachmentName);
							if (regionIter == mapRegions.end())
							{
								mesh->rgbTransformRendererObject = nullptr;
							}
							else
							{
								mesh->rgbTransformRendererObject = regionIter->second;
							}
						}
						break;
					}
				}
			}
		}
	}
}
