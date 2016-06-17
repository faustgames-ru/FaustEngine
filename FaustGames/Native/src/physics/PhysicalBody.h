#ifndef PHYSICAL_BODY_H
#define PHYSICAL_BODY_H

#include "physics_classes.h"
#include "PhysicalConverter.h"

namespace physics
{
	class PhysicalContactIterator: public llge::IPhysicalContactIterator
	{
	public:
		b2Body* body;
		b2ContactEdge* contactEdge;
		b2WorldManifold worldManifold;
		void init(b2ContactEdge* contact);
		virtual void API_CALL CalcWorldManifold() OVERRIDE;
		virtual float API_CALL getWorldNormalX() OVERRIDE;
		virtual float API_CALL getWorldNormalY() OVERRIDE;
		virtual bool API_CALL isEnabled() OVERRIDE;
		virtual bool API_CALL isSensorA() OVERRIDE;
		virtual uint API_CALL getCollisionGroupA() OVERRIDE;
		virtual IntPtr API_CALL getNativeBodyA() OVERRIDE;
		virtual float API_CALL getNativeBodyAPositionX() OVERRIDE;
		virtual float API_CALL getNativeBodyAPositionY() OVERRIDE;
		virtual IntPtr API_CALL getNativeFixtureA() OVERRIDE;
		virtual bool API_CALL isSensorB() OVERRIDE;
		virtual uint API_CALL getCollisionGroupB() OVERRIDE;
		virtual IntPtr API_CALL getNativeBodyB() OVERRIDE;
		virtual float API_CALL getNativeBodyBPositionX() OVERRIDE;
		virtual float API_CALL getNativeBodyBPositionY() OVERRIDE;
		virtual IntPtr API_CALL getNativeFixtureB() OVERRIDE;
		virtual void API_CALL reset() OVERRIDE;
		virtual bool API_CALL isEnd() OVERRIDE;
		virtual void API_CALL next() OVERRIDE;
	};

	class PhysicalBody : public llge::IPhysicalBody
	{
	public:
		b2Body* body;
		int bodyIndex;
		PhysicalBody(b2Body* bodyInstance, PhysicalConverter dimensions, PhysicalConverter velocity);
		~PhysicalBody();
		
		PhysicalFixture* createFixture(llge::PhysicsFixtureConfig config, PhysicalShape* shape);
		void destroyFixture(PhysicalFixture* fixture);

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		virtual float API_CALL getVelocityX() OVERRIDE;
		virtual float API_CALL getVelocityY() OVERRIDE;
		virtual float API_CALL getX() OVERRIDE;
		virtual float API_CALL getY() OVERRIDE;
		virtual float API_CALL getRotation() OVERRIDE;
		virtual llge::IPhysicalContactIterator* API_CALL getContactIterator() OVERRIDE;
		virtual llge::IPhysicalFixture* API_CALL createEdgeFixture(float x0, float y0, float x1, float y1, llge::PhysicsFixtureConfig config) OVERRIDE;
		virtual llge::IPhysicalFixture* API_CALL createCircleFixture(float x, float y, float r, llge::PhysicsFixtureConfig config) OVERRIDE;
		virtual llge::IPhysicalFixture* API_CALL createBoxFixture(float x, float y, float rx, float ry, float rotation, llge::PhysicsFixtureConfig config) OVERRIDE;
		virtual llge::IPhysicalFixture* API_CALL createPolygonFixture(IntPtr vertices2f, int count, llge::PhysicsFixtureConfig config) OVERRIDE;

	private:
		std::vector<PhysicalFixture *> _fistures;
		PhysicalConverter _dimensions;
		PhysicalConverter _velocity;
		PhysicalContactIterator _contactsIterator;
	};
}

#endif /*PHYSICAL_BODY_H*/