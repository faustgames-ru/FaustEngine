#include "Frustum.h"
#include "Aabb.h"

namespace geometry
{
	FrustumPlane::FrustumPlane(): a(0), b(0), c(0), d(0)
	{
	}

	FrustumPlane::FrustumPlane(float p0, float p1, float p2, float p3): a(p0), b(p1), c(p2), d(p3)
	{
	}

	Frustum::Frustum()
	{
		float clip[16]
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
		_planes[0] = FrustumPlane(clip[3] - clip[0], clip[7] - clip[4], clip[11] - clip[8], clip[15] - clip[12]);
		_planes[1] = FrustumPlane(clip[3] + clip[0], clip[7] + clip[4], clip[11] + clip[8], clip[15] + clip[12]);
		_planes[2] = FrustumPlane(clip[3] + clip[1], clip[7] + clip[5], clip[11] + clip[9], clip[15] + clip[13]);
		_planes[3] = FrustumPlane(clip[3] - clip[1], clip[7] - clip[5], clip[11] - clip[9], clip[15] - clip[13]);
	}

	Frustum::Frustum(const core::Matrix& m)
	{
		const float *clip = m.getData();
		_planes[0] = FrustumPlane(clip[3] - clip[0], clip[7] - clip[4], clip[11] - clip[8], clip[15] - clip[12]);
		_planes[1] = FrustumPlane(clip[3] + clip[0], clip[7] + clip[4], clip[11] + clip[8], clip[15] + clip[12]);
		_planes[2] = FrustumPlane(clip[3] + clip[1], clip[7] + clip[5], clip[11] + clip[9], clip[15] + clip[13]);
		_planes[3] = FrustumPlane(clip[3] - clip[1], clip[7] - clip[5], clip[11] - clip[9], clip[15] - clip[13]);
	}

	bool Frustum::include(const Aabb2d& b, float z) const
	{
		for (int p = 0; p < 4; p++)
		{
			if (_planes[p].a*b.Min.getX() + _planes[p].b*b.Min.getY() + _planes[p].c*z + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Max.getX() + _planes[p].b*b.Min.getY() + _planes[p].c*z + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Min.getX() + _planes[p].b*b.Max.getY() + _planes[p].c*z + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Max.getX() + _planes[p].b*b.Max.getY() + _planes[p].c*z + _planes[p].d > 0) continue;							
						
			return false;
		}
		return true;
	}

	bool Frustum::include(const Aabb& b)
	{
		for (int p = 0; p < 4; p++)
		{
			if (_planes[p].a*b.Min.getX() + _planes[p].b*b.Min.getY() + _planes[p].c*b.Min.getZ() + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Max.getX() + _planes[p].b*b.Min.getY() + _planes[p].c*b.Min.getZ() + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Min.getX() + _planes[p].b*b.Max.getY() + _planes[p].c*b.Min.getZ() + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Max.getX() + _planes[p].b*b.Max.getY() + _planes[p].c*b.Min.getZ() + _planes[p].d > 0) continue;
							
			if (_planes[p].a*b.Min.getX() + _planes[p].b*b.Min.getY() + _planes[p].c*b.Max.getZ() + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Max.getX() + _planes[p].b*b.Min.getY() + _planes[p].c*b.Max.getZ() + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Min.getX() + _planes[p].b*b.Max.getY() + _planes[p].c*b.Max.getZ() + _planes[p].d > 0) continue;
			if (_planes[p].a*b.Max.getX() + _planes[p].b*b.Max.getY() + _planes[p].c*b.Max.getZ() + _planes[p].d > 0) continue;

			return false;
		}
		return true;
	}
}
