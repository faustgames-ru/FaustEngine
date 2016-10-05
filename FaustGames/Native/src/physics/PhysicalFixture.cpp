#include "PhysicalFixture.h"
#include "PhysicalFactory.h"

namespace physics
{
	PhysicalFixture::PhysicalFixture(b2Fixture* fixtureInstance, PhysicalConverter dimensions)
		: contactsCounter(0), index(-1), fixture(fixtureInstance), _collisionGroup(fixture->GetFilterData().maskBits), _raycastGroup(0x1), _dimensions(dimensions)
	{
		fixtureInstance->SetUserData(static_cast<void*>(this));
	}

	PhysicalFixture::~PhysicalFixture()
	{
	}

	bool b2TestOverlap(const b2Shape* shapeA, int32 indexA,
		const b2Shape* shapeB, int32 indexB,
		const b2Transform& xfA, const b2Transform& xfB,
		b2DistanceOutput &output)
	{
		b2DistanceInput input;
		input.proxyA.Set(shapeA, indexA);
		input.proxyB.Set(shapeB, indexB);
		input.transformA = xfA;
		input.transformB = xfB;
		input.useRadii = true;

		b2SimplexCache cache;
		cache.count = 0;

		b2Distance(&output, &cache, &input);

		return output.distance < 10.0f * b2_epsilon;
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

	bool PhysicalFixture::testPolygonOverlapEx(float x, float y, IntPtr polygon2f, uint count, IntPtr contactPoint)
	{
		b2PolygonShape poly;
		core::Vector2* points = static_cast<core::Vector2*>(polygon2f);
		PhysicalFactory::initPolygon(points, count, _dimensions, &poly);
		b2Transform transform;
		transform.Set(b2Vec2(_dimensions.toWorld(x), _dimensions.toWorld(y)), 0.0f);
		b2DistanceOutput output;
		bool result = b2TestOverlap((b2Shape*)&poly, 0, fixture->GetShape(), 0, transform, fixture->GetBody()->GetTransform(), output);

		core::Vector2* contact = static_cast<core::Vector2*>(contactPoint);
		contact->setX(_dimensions.fromWorld(output.pointA.x + output.pointB.x)*0.5f);
		contact->setY(_dimensions.fromWorld(output.pointA.y + output.pointB.y)*0.5f);

		return result;
	}

	bool PhysicalFixture::testOverlapEx(IPhysicalFixture* f, IntPtr contactPoint)
	{
		PhysicalFixture* other = static_cast<PhysicalFixture*>(f->getNativeInstance());
		b2Fixture* otherF = other->fixture;
		b2DistanceOutput output;
		bool result = b2TestOverlap(otherF->GetShape(), 0, fixture->GetShape(), 0, otherF->GetBody()->GetTransform(), fixture->GetBody()->GetTransform(), output);
		
		core::Vector2* contact = static_cast<core::Vector2*>(contactPoint);
		contact->setX(_dimensions.fromWorld(output.pointA.x + output.pointB.x)*0.5f);
		contact->setY(_dimensions.fromWorld(output.pointA.y + output.pointB.y)*0.5f);
		return result;
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

	void PhysicalFixture::setRaycastGroup(ushort value)
	{
		_raycastGroup = value;
	}

	ushort PhysicalFixture::getRaycastGroup()
	{
		return _raycastGroup;
	}
}