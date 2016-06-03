#include "PhysicalFixture.h"

namespace physics
{
	PhysicalFixture::PhysicalFixture(b2Fixture* fixtureInstance)
		: fixture(fixtureInstance), index(-1);
	{
	}

	PhysicalFixture::~PhysicalFixture()
	{
	}
}