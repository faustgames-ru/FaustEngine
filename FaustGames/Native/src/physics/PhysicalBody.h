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
	private:
		std::vector<PhysicalFixture *> _fistures;
		PhysicalConverter _dimensions;
		PhysicalConverter _velocity;
	};
}

#endif /*PHYSICAL_BODY_H*/