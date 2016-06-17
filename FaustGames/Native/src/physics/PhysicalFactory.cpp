#include "PhysicalFactory.h"
#include "PhysicalWorld.h"
#include "PhysicalShape.h"

namespace physics
{
	core::Vector2 PhysicalFactory::_verticesScaleBuffer[b2_maxPolygonVertices];


	PhysicalWorld* PhysicalFactory::createWorld()
	{
		return new PhysicalWorld(Dimensions, Velocity);
	}

	PhysicalShape* PhysicalFactory::createCircle(float x, float y, float r)
	{
		PhysicalCircle* c = new PhysicalCircle();
		initCircle(x, y, r, Dimensions, c);
		return c;
	}

	PhysicalShape* PhysicalFactory::createCircle(float r)
	{
		PhysicalCircle* c = new PhysicalCircle();
		initCircle(r, Dimensions, c);
		return c;
	}

	PhysicalShape* PhysicalFactory::createBox(float rx, float ry)
	{
		PhysicalPolygon *p = new PhysicalPolygon();
		initBox(rx, ry, Dimensions, p);
		return p;
	}

	PhysicalShape* PhysicalFactory::createBox(float x, float y, float rx, float ry, float rotation)
	{
		PhysicalPolygon *p = new PhysicalPolygon();
		initBox(rx, ry, rx, ry, rotation, Dimensions, p);
		return p;
	}

	PhysicalShape* PhysicalFactory::createPolygon(const core::Vector2* points, int count)
	{
		PhysicalPolygon *p = new PhysicalPolygon();
		initPolygon(points, count, Dimensions, p);
		return p;
	}

	void PhysicalFactory::initEdge(float x1, float y1, float x2, float y2, PhysicalConverter dimensions, PhysicalEdge* edge)
	{
		edge->setEdge(dimensions.toWorld(x1), dimensions.toWorld(y1), dimensions.toWorld(x2), dimensions.toWorld(y2));
	}

	void PhysicalFactory::initCircle(float x, float y, float r, PhysicalConverter dimensions, PhysicalCircle* circle)
	{
		circle->setCircle(dimensions.toWorld(x), dimensions.toWorld(y), dimensions.toWorld(r));
	}

	void PhysicalFactory::initCircle(float r, PhysicalConverter dimensions, PhysicalCircle* circle)
	{
		circle->setCircle(0, 0, dimensions.toWorld(r));
	}

	void PhysicalFactory::initBox(float rx, float ry, PhysicalConverter dimensions, PhysicalPolygon* polygon)
	{
		polygon->setBox(dimensions.toWorld(rx), dimensions.toWorld(ry));

	}

	void PhysicalFactory::initBox(float x, float y, float rx, float ry, float rotation, PhysicalConverter dimensions, PhysicalPolygon* polygon)
	{
		polygon->setBox(
			dimensions.toWorld(x),
			dimensions.toWorld(y),
			dimensions.toWorld(rx),
			dimensions.toWorld(ry),
			rotation);
	}

	void PhysicalFactory::initPolygon(const core::Vector2* points, int count, PhysicalConverter dimensions, PhysicalPolygon* polygon)
	{
		if (count > b2_maxPolygonVertices)
			count = b2_maxPolygonVertices;
		for (int i = 0; i < count; i++)
		{
			_verticesScaleBuffer[i].setX(dimensions.toWorld(points[i].getX()));
			_verticesScaleBuffer[i].setY(dimensions.toWorld(points[i].getY()));
		}
		polygon->setPolygon(_verticesScaleBuffer, count);
	}

	void PhysicalFactory::initPolygon(const core::Vector2* points, int count, PhysicalConverter dimensions, b2PolygonShape* polygon)
	{
		if (count > b2_maxPolygonVertices)
			count = b2_maxPolygonVertices;
		for (int i = 0; i < count; i++)
		{
			_verticesScaleBuffer[i].setX(dimensions.toWorld(points[i].getX()));
			_verticesScaleBuffer[i].setY(dimensions.toWorld(points[i].getY()));
		}
		const void* ptr = _verticesScaleBuffer;
		polygon->Set(static_cast<const b2Vec2*>(ptr), count);
	}

	PhysicalFactory::PhysicalFactory(float scaleDimensions, float scaleVelocity)
	{
		Dimensions.setup(scaleDimensions);
		Velocity.setup(scaleVelocity);
	}

	PhysicalFactory::~PhysicalFactory()
	{
	}

	llge::IPhysicalWorld* PhysicalFactory::createPhysicalWorld()
	{
		return createWorld();
	}

	llge::IPhysicalShape* PhysicalFactory::createCircleShape(float x, float y, float r)
	{
		return createCircle(x, y, r);
	}

	llge::IPhysicalShape* PhysicalFactory::createBoxShape(float x, float y, float rx, float ry, float rotation)
	{
		return createBox(x, y, rx, ry, rotation);
	}

	llge::IPhysicalShape* PhysicalFactory::createPolygonShape(IntPtr vertices2f, int count)
	{
		return createPolygon(static_cast<const core::Vector2*>(vertices2f), count);
	}

	void PhysicalFactory::dispose()
	{
		delete this;
	}
}