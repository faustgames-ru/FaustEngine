#ifndef GEOMETRY_AABB_H
#define GEOMETRY_AABB_H

#include "geometry_classes.h"

namespace geometry
{
	class Aabb
	{
	public:
		static Aabb infinity;
		core::Vector3 Min;
		core::Vector3 Max;
		Aabb();
		Aabb(core::Vector3 min, core::Vector3 max);
		Aabb(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void divide(Aabb& left, Aabb& right, int axis) const;
		void divide(Aabb& left, Aabb& right) const;
		void expand(float scale);
		void expandX(float x);
		void expandY(float y);
		void expandZ(float z);
		void expand(const core::Vector3 v);
		void expand(float x, float y, float z);
		bool contains(const Aabb& aabb) const;
		bool contains(const core::Vector3& v) const;
		static bool cross(const Aabb& aabb1, const Aabb& aabb2);
	};
}

#endif /*GEOMETRY_AABB_H*/
