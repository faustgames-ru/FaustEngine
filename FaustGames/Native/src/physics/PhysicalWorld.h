#ifndef PHYSICAL_WORLD_H
#define PHYSICAL_WORLD_H

#include "physics_classes.h"
#include "PhysicalConverter.h"
#include "PhysicalBody.h"
#include "PhysicalJoint.h"

namespace physics
{

	class DebugRenderCallback : public b2QueryCallback
	{
	public:
		PhysicalWorld * _world;
		PhysicalConverter _dimensions;
		DebugRenderCallback(PhysicalWorld* world, PhysicalConverter dimensions);
		virtual bool ReportFixture(b2Fixture* fixture) OVERRIDE;
	};

	class RayCastFirstCallback : public b2RayCastCallback
	{
	public:
		uint mask;
		b2Fixture* best;
		float bestFraction;
		core::Vector2 bestPoint;
		core::Vector2 bestNormal;
		bool ignoreSensors;
		RayCastFirstCallback();

		void init(uint maskBits, bool ignoreSensors);
		virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) OVERRIDE;
	};

	class PlatformsContactListener : public b2ContactListener
	{
	public:
		uint platformsCategory;
		PlatformsContactListener();
		virtual void BeginContact(b2Contact* contact) OVERRIDE;
		virtual void EndContact(b2Contact* contact) OVERRIDE;
	};

	class PhysicalWorld : public llge::IPhysicalWorld
	{
	public:	
		PhysicalWorld(PhysicalConverter dimensions, PhysicalConverter velocity);
		~PhysicalWorld();

		void setGravity(float x, float y);
		PhysicalBody* createBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation);
		void destroyBody(PhysicalBody* body);
		PhysicalFixedJoint* createFixedJoint(PhysicalBody* groundBody, PhysicalBody* body, float x, float y, float maxForce);
		void destroyJoint(PhysicalJoint* joint);

		uint getDebugFixtureColor(b2Fixture * fixture);
		void debugRender();
		void debugRenderBody(b2Body * body);
		void debugRenderFixture(core::Vector2 center, float sin, float cos, b2Fixture * fixture);
		void debugRenderShape(uint color, core::Vector2 center, float sin, float cos, b2Shape * shape);

		bool rayCastFirst(float x0, float y0, float x1, float y1, uint mask, bool ignoreSensors, core::Vector2 &result, core::Vector2 &resultNormal);

		virtual void API_CALL debugRender(float x, float y, float rx, float ry) OVERRIDE;
		virtual bool API_CALL makeRayCastFirst(float x0, float y0, float x1, float y1, uint mask, bool ignoreSensors, IntPtr resultPoint, IntPtr resultNormal) OVERRIDE;
		virtual llge::IPhysicalBody* API_CALL createPhysicalBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation) OVERRIDE;
		virtual void API_CALL disposePhysicalBody(llge::IPhysicalBody* body) OVERRIDE;
		virtual llge::IPhysicalFixedJoint* API_CALL createPhysicalFixedJoint(llge::IPhysicalBody* ground, llge::IPhysicalBody* body, float x, float y, float maxForce) OVERRIDE;
		virtual void API_CALL disposePhysicalJoint(llge::IPhysicalFixedJoint* joint) OVERRIDE;
		virtual void API_CALL step(float dt, int velocityIterations, int positionIterations) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
	private:
		b2World* _world;
		std::vector<PhysicalBody*> _bodies;
		std::vector<PhysicalJoint*> _joints;
		PhysicalConverter _dimensions;
		PhysicalConverter _velocity;
		DebugRenderCallback* _debugRenderCallback;
		RayCastFirstCallback _raycastFirst;
		PlatformsContactListener _platformsContactListener;
	};
}

#endif /*PHYSICAL_WORLD_H*/