#ifndef AABB_2D_H
#define AABB_2D_H

#include "stdafx.h"
#include "Vector.h"
#include "Math.h"

namespace geometry
{
	class Aabb2d
	{
	public:
		core::Vector2 Min;
		core::Vector2 Max;

		Aabb2d() :
			Min(core::Math::MaxValue, core::Math::MaxValue),
			Max(-core::Math::MaxValue, -core::Math::MaxValue)
		{
		}

		inline void expand(float x, float y)
		{
			if (Min.getX() < x)
				Min.setX(x);
			if (Min.getY() < y)
				Min.setY(y);
		}

		inline bool contains(const core::Vector2 &v) const
		{
			return
				(Min.getX() <= v.getX()) && (v.getX() <= Max.getX()) &&
				(Min.getY() <= v.getY()) && (v.getY() <= Max.getY());
		}

		inline static bool cross(const Aabb2d &aabb1, const Aabb2d &aabb2)
		{
			return
				aabb1.contains(aabb2.Min) || aabb1.contains(aabb2.Max) ||
				aabb2.contains(aabb1.Min) || aabb2.contains(aabb1.Max);
		}
	};
}

#endif /*AABB*/
