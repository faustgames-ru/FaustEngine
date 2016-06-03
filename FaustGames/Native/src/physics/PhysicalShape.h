#ifndef PHYSICAL_SHAPE_H
#define PHYSICAL_SHAPE_H

#include "physics_classes.h"

namespace physics
{
	class PhysicalShape
	{
	public:
		PhysicalShape();
		virtual ~PhysicalShape();
		virtual b2Shape* getShape() = 0;
	private:
	};

	class PhysicalCircle: PhysicalShape
	{
	public:
		b2Shape* getShape() OVERRIDE;
		PhysicalCircle(float worldX, float worldY, float wolrdR);
	private:
		b2CircleShape _shape;
	};

	class PhysicalPolygon : PhysicalShape
	{
	public:
		PhysicalPolygon();
		PhysicalPolygon(const core::Vector2 *points, int count);
		void setPolygon(const core::Vector2 *points, int count);
		void setBox(float rx, float ry);
		void setBox(float x, float y, float rx, float ry, float rotation);
		b2Shape* getShape() OVERRIDE;
	private:
		b2PolygonShape _shape;
	};
}

#endif /*PHYSICAL_FIXTURE_H*/