#include "PhysicalFixture.h"

namespace physics
{
	PhysicalFixture::PhysicalFixture(b2Fixture* fixtureInstance)
		: index(-1), fixture(fixtureInstance)
	{
	}

	PhysicalFixture::~PhysicalFixture()
	{
	}

	IntPtr PhysicalFixture::getNativeInstance()
	{
		return this;
	}

	float PhysicalFixture::getDensity()
	{
		return fixture->GetDensity();
	}

	float PhysicalFixture::getFriction()
	{
		return fixture->GetFriction();
	}

	float PhysicalFixture::getBonce()
	{
		return fixture->GetRestitution();
	}

	bool PhysicalFixture::isSensor()
	{
		return fixture->IsSensor();
	}

	uint PhysicalFixture::getCollidesWith()
	{
		return fixture->GetFilterData().maskBits;
	}

	uint PhysicalFixture::getCollisionGroup()
	{
		return fixture->GetFilterData().categoryBits;
	}

	void PhysicalFixture::setDensity(float value)
	{
		fixture->SetDensity(value);
	}

	void PhysicalFixture::setFriction(float value)
	{
		fixture->SetFriction(value);
	}

	void PhysicalFixture::setBonce(float value)
	{
		fixture->SetRestitution(value);
	}

	void PhysicalFixture::setSensor(bool value)
	{
		fixture->SetSensor(value);
	}

	void PhysicalFixture::setCollidesWith(uint value)
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = value;
		fixture->SetFilterData(filter);
	}

	void PhysicalFixture::setCollisionGroup(uint value)
	{
		b2Filter filter = fixture->GetFilterData();
		filter.categoryBits = value;
		fixture->SetFilterData(filter);
	}
}