#include "PhysicalBody.h"
#include "PhysicalShape.h"
#include "PhysicalFixture.h"

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
		fixtureDef.isSensor = config.isSensor;
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
}