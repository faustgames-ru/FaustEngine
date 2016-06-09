#ifndef PHYSICAL_BODY_H
#define PHYSICAL_BODY_H

#include "physics_classes.h"
#include "PhysicalConverter.h"

namespace physics
{
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
		virtual llge::IPhysicalFixture* API_CALL createCircleFixture(float x, float y, float r, llge::PhysicsFixtureConfig config, IntPtr userData) OVERRIDE;
		virtual llge::IPhysicalFixture* API_CALL createBoxFixture(float x, float y, float rx, float ry, float rotation, llge::PhysicsFixtureConfig config, IntPtr userData) OVERRIDE;
		virtual llge::IPhysicalFixture* API_CALL createPolygonFixture(IntPtr vertices2f, int count, llge::PhysicsFixtureConfig config, IntPtr userData) OVERRIDE;

	private:
		std::vector<PhysicalFixture *> _fistures;
		PhysicalConverter _dimensions;
		PhysicalConverter _velocity;
	};
}

#endif /*PHYSICAL_BODY_H*/