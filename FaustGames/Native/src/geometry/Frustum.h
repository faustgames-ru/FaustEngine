#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "geometry_classes.h"
#include "Aabb2d.h"
#include "Aabb.h"

namespace geometry
{
	class FrustumPlane
	{
	public:
		float a;
		float b;
		float c;
		float d;
		FrustumPlane();
		FrustumPlane(float p0, float p1, float p2, float p3);
	};

	class Frustum
	{
	public:
		Frustum();
		Frustum(const core::Matrix &m);
		bool include(const Aabb2d &aabb, float z) const;
		bool include(const Aabb& b);
	private:
		FrustumPlane _planes[4];
		
	};
}

#endif /*FRUSTUM_H*/
