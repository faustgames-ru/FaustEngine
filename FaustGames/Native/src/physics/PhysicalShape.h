#ifndef PHYSICAL_SHAPE_H
#define PHYSICAL_SHAPE_H

#include "physics_classes.h"

namespace physics
{
	class PhysicalShape : public llge::IPhysicalShape
	{
	public:
		PhysicalShape();
		virtual ~PhysicalShape();
		virtual b2Shape* getShape() = 0;

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
	private:
	};

	class PhysicalEdge : public PhysicalShape
	{
	public:
		b2Shape* getShape() OVERRIDE;
		PhysicalEdge();
		void setEdge(float worldX1, float worldY1, float worldX2, float worldY2);
		void setV0(float worldX0, float worldY0);
		void setV3(float worldX0, float worldY0);
	private:
		b2EdgeShape _shape;
	};
	
	class PhysicalCircle: public PhysicalShape
	{
	public:
		b2Shape* getShape() OVERRIDE;
		PhysicalCircle();
		PhysicalCircle(float worldX, float worldY, float wolrdR);
		void setCircle(float worldX, float worldY, float wolrdR);
	private:
		b2CircleShape _shape;
	};

	class PhysicalPolygon : public PhysicalShape
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

#endif /*PHYSICAL_SHAPE_H*/