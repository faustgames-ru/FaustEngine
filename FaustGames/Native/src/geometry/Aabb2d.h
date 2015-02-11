#ifndef AABB_2D_H
#define AABB_2D_H

#include "geometry_classes.h"

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

		Aabb2d(float minX, float minY, float maxX, float maxY) :
			Min(minX, minY),
			Max(maxX, maxY)
		{
		}

		inline void divide(Aabb2d &left, Aabb2d &right) const
		{
			float sizeX = Max.getX() - Min.getX();
			float sizeY = Max.getY() - Min.getY();
			if (sizeX > sizeY)
			{
				left.Min.setX(Min.getX());
				left.Min.setY(Min.getY());
				left.Max.setX(Min.getX() + sizeX * 0.5f);
				left.Max.setY(Max.getY());

				right.Min.setX(Min.getX() + sizeX * 0.5f);
				right.Min.setY(Min.getY());
				right.Max.setX(Max.getX());
				right.Max.setY(Max.getY());
			}
			else
			{
				left.Min.setX(Min.getX());
				left.Min.setY(Min.getY());
				left.Max.setX(Max.getX());
				left.Max.setY(Min.getY() + sizeY * 0.5f);

				right.Min.setX(Min.getX());
				right.Min.setY(Min.getY() + sizeY * 0.5f);
				right.Max.setX(Max.getX());
				right.Max.setY(Max.getY());
			}
		}
		

		inline void expand(float x, float y)
		{
			if (Min.getX() < x)
				Min.setX(x);
			if (Min.getY() < y)
				Min.setY(y);
		}

		inline bool contains(const Aabb2d &aabb) const
		{
			return contains(aabb.Min) && contains(aabb.Max);
		}
		
		inline bool contains(const core::Vector2 &v) const
		{
			return
				(Min.getX() <= v.getX()) && (v.getX() <= Max.getX()) &&
				(Min.getY() <= v.getY()) && (v.getY() <= Max.getY());
		}

		inline static bool cross(const Aabb2d &aabb1, const Aabb2d &aabb2)
		{
			if ((aabb1.Max.getX() < aabb2.Min.getX()) || (aabb1.Min.getX() > aabb2.Max.getX())) return false;
			if ((aabb1.Max.getY() < aabb2.Min.getY()) || (aabb1.Min.getY() > aabb2.Max.getY())) return false;

			return true;
		}
	};
}

#endif /*AABB*/
