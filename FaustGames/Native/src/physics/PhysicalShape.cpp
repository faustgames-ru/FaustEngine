#include "PhysicalShape.h"

namespace physics
{
	PhysicalShape::PhysicalShape()
	{
	}

	PhysicalShape::~PhysicalShape()
	{
	}

	IntPtr PhysicalShape::getNativeInstance()
	{
		return this;
	}

	void PhysicalShape::dispose()
	{
		delete this;
	}

	b2Shape* PhysicalCircle::getShape()
	{
		return &_shape;
	}

	PhysicalCircle::PhysicalCircle()
	{
	}

	PhysicalCircle::PhysicalCircle(float worldX, float worldY, float wolrdR)
	{
		_shape.m_p = b2Vec2(worldX, worldY);
		_shape.m_radius = wolrdR;
	}

	void PhysicalCircle::setCircle(float worldX, float worldY, float wolrdR)
	{
		_shape.m_p = b2Vec2(worldX, worldY);
		_shape.m_radius = wolrdR;
	}

	PhysicalPolygon::PhysicalPolygon()
	{
	}

	PhysicalPolygon::PhysicalPolygon(const core::Vector2* points, int count)
	{
		setPolygon(points, count);
	}

	void PhysicalPolygon::setPolygon(const core::Vector2 * points, int count)
	{
		const void* ptr = points;
		_shape.Set(static_cast<const b2Vec2*>(ptr), count);
	}

	void PhysicalPolygon::setBox(float rx, float ry)
	{
		_shape.SetAsBox(rx, ry);
	}

	void PhysicalPolygon::setBox(float x, float y, float rx, float ry, float rotation)
	{
		_shape.SetAsBox(rx, ry, b2Vec2(x, y), rotation);
	}

	b2Shape* PhysicalPolygon::getShape()
	{
		return &_shape;
	}
}