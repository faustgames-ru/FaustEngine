#ifndef PHYSICAL_FACTORY_H
#define PHYSICAL_FACTORY_H

#include "physics_classes.h"
#include "PhysicalConverter.h"
#include "PhysicalShape.h"

namespace physics
{

	class PhysicalFactory : public llge::IPhysicalFactory
	{
	public:
		PhysicalConverter Dimensions;
		PhysicalConverter Velocity;

		PhysicalWorld* createWorld();
		PhysicalShape* createCircle(float x, float y, float r);
		PhysicalShape* createCircle(float r);
		PhysicalShape* createBox(float rx, float ry);
		PhysicalShape* createBox(float x, float y, float rx, float ry, float rotation);
		PhysicalShape* createPolygon(const core::Vector2* points, int count);
		static void initEdge(float x1, float y1, float x2, float y2, PhysicalConverter dimensions, PhysicalEdge* edge);
		static void initCircle(float x, float y, float r, PhysicalConverter dimensions, PhysicalCircle* circle);
		static void initCircle(float r, PhysicalConverter dimensions, PhysicalCircle* circle);
		static void initBox(float rx, float ry, PhysicalConverter dimensions, PhysicalPolygon* polygon);
		static void initBox(float x, float y, float rx, float ry, float rotation, PhysicalConverter dimensions, PhysicalPolygon* polygon);
		static void initPolygon(const core::Vector2* points, int count, PhysicalConverter dimensions, PhysicalPolygon* polygon);
		static void initPolygon(const core::Vector2* points, int count, PhysicalConverter dimensions, b2PolygonShape* polygon);

		PhysicalFactory(float scaleDimensions, float scaleVelocity);
		~PhysicalFactory();

		virtual llge::IPhysicalWorld* API_CALL createPhysicalWorld() OVERRIDE;
		virtual llge::IPhysicalShape* API_CALL createCircleShape(float x, float y, float r) OVERRIDE;
		virtual llge::IPhysicalShape* API_CALL createBoxShape(float x, float y, float rx, float ry, float rotation) OVERRIDE;
		virtual llge::IPhysicalShape* API_CALL createPolygonShape(IntPtr vertices2f, int count) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
	private:
		static core::Vector2 _verticesScaleBuffer[b2_maxPolygonVertices];
	};
}

#endif /*PHYSICAL_FACTORY_H*/