#ifndef SPINE_SKELETON_RESOURCE_H
#define SPINE_SKELETON_RESOURCE_H

#include "spine_classes.h"

struct spAtlasRegion;
struct spSkeleton;

namespace spine
{
	class SpineEvent : public llge::ISpineEvent
	{
	public:
		virtual IntPtr API_CALL getName();
		SpineEvent(const char * name);
	private:
		std::string _name;
	};

	class SpineSkin : public llge::ISpineSkin
	{
	public:
		virtual IntPtr API_CALL getName() OVERRIDE;
		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		SpineSkin(void * spSkin);
	private:
		std::string _name;
		void * _spSkin;
	};
	
	class SpineDynamicSkin : public llge::ISpineDynamicSkin
	{
	public:
		virtual llge::ISpineSkin* API_CALL skinValue() OVERRIDE;
		virtual void API_CALL addSkin(llge::ISpineSkin*) OVERRIDE;
		void dispose();
		SpineDynamicSkin();
		~SpineDynamicSkin();
	private:
		SpineSkin* _value;
	};

	class AtlasRenderObject
	{
	public:
		llge::TextureQueryFormat pagesFormat;
		float applyedCompression;
		void *texture;
		llge::ITexturesManager* texturesManager;
	};


	class SpineSkeletonResource : public llge::ISpineResource
	{
	public:
		void *getSkeletonData();
		SpineSkeletonAnimation* getAnimation(int i);
		SpineSkeletonAnimation* findAnimation(const char* name);
		int getAnimationsCount();
		SpineEvent* getEvent(int i);
		int getEventsCount();

		void loadInternal(String atlasText, String jsonText, String dir, llge::TextureQueryFormat format, float applyedCompression, void* texture, llge::ITexturesManager* textures);
		virtual void API_CALL load(String atlasText, String jsonText, String dir, llge::TextureQueryFormat format, float applyedCompression, llge::ITexturesManager* textures);
		virtual void API_CALL loadWithPngImage(String atlasText, String jsonText, String dir, void* texture) override;
		virtual void API_CALL unLoad();
		virtual llge::ISpineAnimation* API_CALL getSpineAnimation(int i);
		virtual int API_CALL getSpineAnimationsCount();
		virtual llge::ISpineSkin* API_CALL getSpineSkin(int i);
		virtual int API_CALL getSpineSkinsCount();
		virtual llge::ISpineEvent* API_CALL getSpineEvent(int i);
		virtual int API_CALL getSpineEventsCount();
		virtual llge::ISpineSkeleton* API_CALL createSkeleton(void *floatMatrix);
		virtual llge::ISpineAnimationStateData* API_CALL createStateData();
		virtual llge::ISpineDynamicSkin* API_CALL createDynamicSkin();
		virtual llge::ISpineSkin* API_CALL getDefaultSkin();

		virtual IntPtr API_CALL errorMessage() OVERRIDE;
		virtual bool API_CALL isValid() OVERRIDE;
		
		virtual void API_CALL dispose();

		void InitRgbTransformRegions(int slotCount, const char* RgbTransformName) const;

	protected:
	private:
		void * _spAtlas;
		void *_spSkeletonJson;
		void *_spSkeletonData;
	//	std::map<std::string, std::vector<spSlotData* > > _rgbTransformSlots;
		std::vector<SpineSkeletonAnimation* > _animations;
		std::vector<SpineEvent* > _events;
		std::vector<SpineSkin* > _skins;
		std::vector<SpineDynamicSkin* > _dynamicSkins;
		SpineSkin* _defaultSkin;
		std::string _error;
		bool _invalid;
		AtlasRenderObject _atlasRenderObject;
	};
}

#endif /*SPINE_SKELETON_RESOURCE_H*/