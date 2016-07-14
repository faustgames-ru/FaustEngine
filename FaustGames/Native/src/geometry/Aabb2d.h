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

		Aabb2d();

		Aabb2d(core::Vector2 min, core::Vector2 max);

		Aabb2d(float minX, float minY, float maxX, float maxY);

		void divide(Aabb2d& left, Aabb2d& right) const;


		void expand(float scale);

		void expandX(float x);

		void expand(const core::Vector2 v);

		void expandY(float y);

		void expand(float x, float y);

		bool contains(const Aabb2d& aabb) const;

		bool contains(const core::Vector2& v) const;

		static float squareProportion(const Aabb2d& aabb1, const Aabb2d& aabb2);

		static bool cross(const Aabb2d& aabb1, const Aabb2d& aabb2);
	};
}

#endif /*AABB_2D_H*/
