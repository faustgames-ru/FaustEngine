#ifndef SPINE_SPINE_SKELETON_H
#define SPINE_SPINE_SKELETON_H

#include "spine_classes.h"

namespace spine
{
	class SpineSkeleton : public llge::ISpineSkeleton
	{
	public:
		SpineSkeleton(SpineSkeletonResource *resource, float* transform);
		~SpineSkeleton();
		virtual void API_CALL renderEx(llge::IBatch2d * batch, IntPtr effectConfig, llge::GraphicsEffects effect);
		virtual void API_CALL render(llge::IBatch2d * batch, int lightmapId, llge::GraphicsEffects effect);
		virtual int API_CALL getGeometry(void *vertices, int verticeLimit, void *indices, int indicesLimit);
		void cleanup();
		void updateAabb();
		void transform(float *x, float *y);
		void API_CALL updateWorldTransform();
		void *getSkeleton();
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


	protected:
	private:
		void initFromResource(SpineSkeletonResource *resource);
		void *_spSkeleton;
		geometry::Aabb2d _aabb;
		core::Matrix _transform;
		static drawing::BatcherSpineMesh _mesh;
		static int _quadIndices[6];
		static float _uvBuffer[4096];
		llge::LightingConfig _lightingConfig; // todo: remove
	};
}

#endif /*SPINE_SPINE_SKELETON_H*/