#include "PhysicalBody.h"
#include "PhysicalShape.h"
#include "PhysicalFixture.h"
#include "PhysicalFactory.h"

namespace physics
{
	PhysicalBody::PhysicalBody(b2Body* bodyInstance, PhysicalConverter dimensions, PhysicalConverter velocity): bodyIndex(-1)
	{
		body = bodyInstance;
		_dimensions = dimensions;
		_velocity = velocity;
	}

	PhysicalBody::~PhysicalBody()
	{
		for (int i = 0; i < _fistures.size(); i++)
		{
			body->DestroyFixture(_fistures[i]->fixture);
			delete _fistures[i];
		}
	}

	PhysicalFixture* PhysicalBody::createFixture(llge::PhysicsFixtureConfig config, PhysicalShape* shape)
	{
		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape->getShape();
		fixtureDef.density = config.density;
		fixtureDef.friction = config.friction;
		fixtureDef.restitution = config.bonce;
		fixtureDef.isSensor = config.isSensor != 0;
		fixtureDef.filter.maskBits = config.collidesWith;
		fixtureDef.filter.categoryBits = config.collisionGroup;
		b2Fixture* fixtureInstance = body->CreateFixture(&fixtureDef);
		PhysicalFixture* fixture = new PhysicalFixture(fixtureInstance);
		fixture->index = _fistures.size();
		_fistures.push_back(fixture);
		return fixture;
	}

	void PhysicalBody::destroyFixture(PhysicalFixture* fixture)
	{
		body->DestroyFixture(fixture->fixture);
		int index = fixture->index;
		_fistures[index] = _fistures.back();
		_fistures[index]->index = index;
		_fistures.pop_back();
		delete fixture;
	}

	IntPtr PhysicalBody::getNativeInstance()
	{
		return this;
	}

	float PhysicalBody::getVelocityX()
	{
		return _velocity.fromWorld(body->GetLinearVelocity().x);
	}

	float PhysicalBody::getVelocityY()
	{
		return _velocity.fromWorld(body->GetLinearVelocity().y);
	}

	float PhysicalBody::getX()
	{
		return _dimensions.fromWorld(body->GetPosition().x);
	}

	float PhysicalBody::getY()
	{
		return _dimensions.fromWorld(body->GetPosition().y);
	}

	float PhysicalBody::getRotation()
	{
		return body->GetAngle();
	}

	llge::IPhysicalFixture* PhysicalBody::createCircleFixture(float x, float y, float r, llge::PhysicsFixtureConfig config, IntPtr userData)
	{
		PhysicalCircle circle;
		PhysicalFactory::initCircle(x, y, r, _dimensions, &circle);
		PhysicalFixture* fixture = createFixture(config, &circle);
		fixture->fixture->SetUserData(userData);
		return fixture;
	}

	llge::IPhysicalFixture* PhysicalBody::createBoxFixture(float x, float y, float rx, float ry, float rotation, llge::PhysicsFixtureConfig config, IntPtr userData)
	{
		PhysicalPolygon polygon;
		PhysicalFactory::initBox(x, y, rx, ry, rotation, _dimensions, &polygon);
		PhysicalFixture* fixture = createFixture(config, &polygon);
		fixture->fixture->SetUserData(userData);
		return fixture;
	}

	llge::IPhysicalFixture* PhysicalBody::createPolygonFixture(IntPtr vertices2f, int count, llge::PhysicsFixtureConfig config, IntPtr userData)
	{
		PhysicalPolygon polygon;
		PhysicalFactory::initPolygon(static_cast<const core::Vector2 *>(vertices2f), count, _dimensions, &polygon);
		PhysicalFixture* fixture = createFixture(config, &polygon);
		fixture->fixture->SetUserData(userData);
		return fixture;
	}
}