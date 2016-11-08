#ifndef SPINE_SPINE_SKELETON_H
#define SPINE_SPINE_SKELETON_H

#include "spine_classes.h"

namespace spine
{
	class SpineSkeletonSlot
	{
	public:
		SpineSkeletonSlot(void *slot);
		void setRgb(uint color);
		const char* name();
	private:
		void *_spSlot;
	};
	class SpineSkeletonBone : public llge::ISpineSkeletonBone
	{
	public:
		llge::BoneFx fx;
		int rgbTransfomrIndex;
		SpineSkeletonBone(void *bone);
		const char* name();
		virtual IntPtr API_CALL getName() OVERRIDE;
		virtual float API_CALL getX() OVERRIDE;
		virtual float API_CALL getY() OVERRIDE;
		virtual void API_CALL setBoneFx(llge::BoneFx fx) OVERRIDE;
		virtual void API_CALL setBoneRgbTransfomrIndex(int index) OVERRIDE;

	private:
		void *_spBone;
	};

	class SpineSkeletonBounds
	{
	public:
		float vertices[128];
		int count;
	};

	class SpineSkeleton : public llge::ISpineSkeleton
	{
	public:
		SpineSkeleton(SpineSkeletonResource *resource, float* transform);
		~SpineSkeleton();
		virtual float API_CALL getBoundsVertexX(int boundsIndex, int vertexIndex);
		virtual float API_CALL getBoundsVertexY(int boundsIndex, int vertexIndex);
		virtual int API_CALL getBoundsVerticesCount(int boundsIndex);
		virtual int API_CALL getBoundsCount();
		virtual llge::ISpineSkeletonBone* API_CALL getBone(int index) OVERRIDE;
		virtual int API_CALL getBonesCount() OVERRIDE;
		virtual void API_CALL renderEx(llge::IBatch2d * batch, IntPtr effectConfig, llge::GraphicsEffects effect, byte colorScale);
		virtual void API_CALL render(llge::IBatch2d * batch, int lightmapId, llge::GraphicsEffects effect, byte colorScale);
		virtual int API_CALL getGeometry(void *vertices, int verticeLimit, void *indices, int indicesLimit);
		void cleanup();
		void updateAabb();
		void transform(float *x, float *y);
		void API_CALL updateWorldTransform();
		void *getSkeleton() const;
		SpineSkeletonBone* findBone(const char *boneName);
		virtual IntPtr API_CALL getNativeInstance();
		virtual void API_CALL applySkin(IntPtr spineSkinNativeInstance);
		virtual void API_CALL dispose();
		virtual void API_CALL setTransform(void *floatMatrix);
		virtual void API_CALL setColor(uint color);
		virtual void API_CALL setBonesToSetupPose();
		virtual void API_CALL setSlotsToSetupPose();
		virtual float API_CALL getMinX();
		virtual float API_CALL getMinY();
		virtual float API_CALL getMaxX();
		virtual float API_CALL getMaxY();
		virtual float API_CALL getZ();
		virtual void API_CALL setRgbTransform(int index, void *floatMatrix3) OVERRIDE;
		virtual void API_CALL setDefaultRgbTransform(int index) OVERRIDE;
		SpineSkeletonSlot* findSlot(const char* slotName);
		void setSlotRgb(const char* name, int color);

	protected:
	private:
		static const int _colorTransformCount = 16;
		static std::string _colorTransformNames[16];

		void initFromResource(SpineSkeletonResource *resource);
		void *_spSkeleton;
		geometry::Aabb2d _aabb;
		core::Matrix _transform;
		core::Matrix3 _colorTransform[_colorTransformCount];
		static drawing::BatcherSpineMesh _mesh;
		static ushort _quadIndices[6];
		static float _uvBuffer[65536];
		llge::LightingConfig _lightingConfig; // todo: remove
		std::vector<SpineSkeletonSlot *> _slots;
		std::vector<SpineSkeletonBone *> _bones;
		std::vector<SpineSkeletonBounds> _bounds;
		int _defaultRgbTransformIndex;
	};
}

#endif /*SPINE_SPINE_SKELETON_H*/