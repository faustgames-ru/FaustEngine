#ifndef PHYSICAL_FIXTURE_H
#define PHYSICAL_FIXTURE_H

#include "physics_classes.h"

namespace physics
{
	class PhysicalFixture : public llge::IPhysicalFixture
	{
	public:
		int index;
		b2Fixture* fixture;
		PhysicalFixture(b2Fixture* fixtureInstance);
		~PhysicalFixture();
	private:
	};
}

#endif /*PHYSICAL_FIXTURE_H*/