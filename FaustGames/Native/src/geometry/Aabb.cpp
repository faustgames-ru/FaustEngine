#include "Aabb.h"

namespace geometry
{
	Aabb Aabb::infinity(-core::Math::MaxValue, -core::Math::MaxValue, -core::Math::MaxValue, core::Math::MaxValue, core::Math::MaxValue, core::Math::MaxValue);


	Aabb::Aabb():
		Min(core::Math::MaxValue, core::Math::MaxValue, core::Math::MaxValue),
		Max(-core::Math::MaxValue, -core::Math::MaxValue, -core::Math::MaxValue)
	{
	}

	Aabb::Aabb(core::Vector3 min, core::Vector3 max):
		Min(min),
		Max(max)
	{
	}

	Aabb::Aabb(float minX, float minY, float minZ, float maxX, float maxY, float maxZ):
		Min(minX, minY, minZ),
		Max(maxX, maxY, maxZ)
	{
	}

	void Aabb::divide(Aabb& left, Aabb& right, int axis) const
	{
		for (uint i = 0; i < 3; i++)
		{
			left.Min.getData()[i] = Min.getData()[i];
			left.Max.getData()[i] = Max.getData()[i];
			right.Min.getData()[i] = Min.getData()[i];
			right.Max.getData()[i] = Max.getData()[i];
		}

		float mid = (Min.getData()[axis] + Max.getData()[axis])*0.5f;
		left.Min.getData()[axis] = mid;
		right.Max.getData()[axis] = mid;
	}

	void Aabb::divide(Aabb& left, Aabb& right) const
	{
		core::Vector3 size = Max - Min;
		if (size.getX() > size.getY())
		{				
			if (size.getX() > size.getZ())
			{
				divide(left, right, 0);
			}
			else
			{					
				divide(left, right, 2);
			}
		}
		else
		{
			if (size.getY() > size.getZ())
			{
				divide(left, right, 1);
			}
			else
			{
				divide(left, right, 2);
			}
		}
	}

	void Aabb::expand(float scale)
	{
		core::Vector3 size = core::Vector3((Max.getX() - Min.getX()) * (scale - 1.0f), (Max.getY() - Min.getY()) * (scale - 1.0f), (Max.getZ() - Min.getZ()) * (scale - 1.0f));
		Min.setX(Min.getX() - size.getX());
		Min.setY(Min.getY() - size.getY());
		Min.setZ(Min.getZ() - size.getZ());
		Max.setX(Min.getX() + size.getX());
		Max.setY(Min.getY() + size.getY());
		Max.setZ(Min.getZ() + size.getZ());
	}

	void Aabb::expandX(float x)
	{
		if (Min.getX() > x)
			Min.setX(x);
		if (Max.getX() < x)
			Max.setX(x);
	}

	void Aabb::expandY(float y)
	{
		if (Min.getY() > y)
			Min.setY(y);
		if (Max.getY() < y)
			Max.setY(y);
	}

	void Aabb::expandZ(float z)
	{
		if (Min.getZ() > z)
			Min.setZ(z);
		if (Max.getZ() < z)
			Max.setZ(z);
	}

	void Aabb::expand(const core::Vector3 v)
	{
		expandX(v.getX());
		expandY(v.getY());
		expandZ(v.getZ());
	}

	void Aabb::expand(float x, float y, float z)
	{
		expandX(x);
		expandY(y);
		expandZ(z);
	}

	bool Aabb::contains(const Aabb& aabb) const
	{
		return contains(aabb.Min) && contains(aabb.Max);
	}

	bool Aabb::contains(const core::Vector3& v) const
	{
		return
			(Min.getX() <= v.getX()) && (v.getX() <= Max.getX()) &&
			(Min.getY() <= v.getY()) && (v.getY() <= Max.getY()) &&
			(Min.getZ() <= v.getZ()) && (v.getZ() <= Max.getZ());
	}

	bool Aabb::cross(const Aabb& aabb1, const Aabb& aabb2)
	{
		if (aabb1.Max.getX() < aabb2.Min.getX() || aabb1.Min.getX() > aabb2.Max.getX()) return false;
		if (aabb1.Max.getY() < aabb2.Min.getY() || aabb1.Min.getY() > aabb2.Max.getY()) return false;
		if (aabb1.Max.getZ() < aabb2.Min.getZ() || aabb1.Min.getZ() > aabb2.Max.getZ()) return false;

		return true;
	}
}