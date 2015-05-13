#ifndef SPINE_SPINE_SKELETON_H
#define SPINE_SPINE_SKELETON_H

#include "spine_classes.h"

namespace spine
{
	class SpineSkeleton : public llge::ISpineSkeleton
	{
	public:
		SpineSkeleton(SpineSkeletonResource *resource);
		~SpineSkeleton();
		virtual void API_CALL render(llge::IBatch2d * batch);
		void cleanup();
		void API_CALL updateWorldTransform();
		void *getSkeleton();
		virtual IntPtr API_CALL getNativeInstance();
		virtual void API_CALL dispose();
	protected:
	private:
		void initFromResource(SpineSkeletonResource *resource);
		void *_spSkeleton;
		static drawing::BatcherSpineMesh _mesh;
		static int _quadIndices[6];
		static float _uvBuffer[4096];
	};
}

#endif /*SPINE_SPINE_SKELETON_H*/