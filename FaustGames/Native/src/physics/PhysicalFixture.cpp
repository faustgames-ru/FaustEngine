#include "PhysicalFixture.h"
#include "PhysicalFactory.h"

namespace physics
{
	PhysicalFixture::PhysicalFixture(b2Fixture* fixtureInstance, PhysicalConverter dimensions)
		: contactsCounter(0), index(-1), fixture(fixtureInstance), _collisionGroup(fixture->GetFilterData().maskBits), _dimensions(dimensions)
	{
	}

	PhysicalFixture::~PhysicalFixture()
	{
	}

	bool PhysicalFixture::polygonOverlap(float x, float y, const core::Vector2* points, uint count)
	{
		b2PolygonShape poly;
		PhysicalFactory::initPolygon(points, count, _dimensions, &poly);
		b2Transform transform;
		transform.Set(b2Vec2(_dimensions.toWorld(x), _dimensions.toWorld(y)), 0.0f);
		return b2TestOverlap(&poly, 0, fixture->GetShape(), 0, transform, fixture->GetBody()->GetTransform());
	}

	bool PhysicalFixture::testPolygonOverlap(float x, float y, IntPtr polygon2f, uint count)
	{
		return polygonOverlap(x, y, static_cast<const core::Vector2*>(polygon2f), count);
	}

	bool PhysicalFixture::testOverlap(llge::IPhysicalFixture* f)
	{
		PhysicalFixture* other = static_cast<PhysicalFixture*>(f->getNativeInstance());
		b2Fixture* otherF = other->fixture;
		return b2TestOverlap(otherF->GetShape(), 0, fixture->GetShape(), 0, otherF->GetBody()->GetTransform(), fixture->GetBody()->GetTransform());
	}

	void PhysicalFixture::pauseCollisions(uint group)
	{
		_collisionGroup = fixture->GetFilterData().maskBits;
		setCollidesWith(_collisionGroup & ~group);
	}

	void PhysicalFixture::resumeCollisions()
	{
		setCollidesWith(_collisionGroup);
	}

	IntPtr PhysicalFixture::getNativeInstance()
	{
		return this;
	}

	int PhysicalFixture::getContactsCounter()
	{
		return contactsCounter;
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