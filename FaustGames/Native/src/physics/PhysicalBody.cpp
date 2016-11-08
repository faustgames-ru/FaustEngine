#include "PhysicalBody.h"
#include "PhysicalShape.h"
#include "PhysicalFixture.h"
#include "PhysicalFactory.h"

namespace physics
{
	void PhysicalContactIterator::init(b2ContactEdge* contact)
	{
		contactEdge = contact;
	}

	void PhysicalContactIterator::CalcWorldManifold()
	{
		contactEdge->contact->GetWorldManifold(&worldManifold);
	}

	float PhysicalContactIterator::getWorldNormalX()
	{
		return worldManifold.normal.x;
	}

	float PhysicalContactIterator::getWorldNormalY()
	{
		return worldManifold.normal.y;
	}

	bool PhysicalContactIterator::isEnabled()
	{		
		return contactEdge->contact->IsTouching() && contactEdge->contact->IsEnabled();
	}

	bool PhysicalContactIterator::isSensorA()
	{
		return contactEdge->contact->GetFixtureA()->IsSensor();
	}

	uint PhysicalContactIterator::getCollisionGroupA()
	{
		return contactEdge->contact->GetFixtureA()->GetFilterData().categoryBits;
	}

	IntPtr PhysicalContactIterator::getNativeBodyA()
	{
		return contactEdge->contact->GetFixtureA()->GetBody()->GetUserData();
	}

	float PhysicalContactIterator::getNativeBodyAPositionX()
	{
		return static_cast<PhysicalBody*>(contactEdge->contact->GetFixtureA()->GetBody()->GetUserData())->getX();
	}

	float PhysicalContactIterator::getNativeBodyAPositionY()
	{
		return static_cast<PhysicalBody*>(contactEdge->contact->GetFixtureA()->GetBody()->GetUserData())->getY();
	}

	IntPtr PhysicalContactIterator::getNativeFixtureA()
	{
		return contactEdge->contact->GetFixtureA()->GetUserData();
	}

	bool PhysicalContactIterator::isSensorB()
	{
		return contactEdge->contact->GetFixtureB()->IsSensor();
	}

	uint PhysicalContactIterator::getCollisionGroupB()
	{
		return contactEdge->contact->GetFixtureB()->GetFilterData().categoryBits;
	}

	IntPtr PhysicalContactIterator::getNativeBodyB()
	{
		return contactEdge->contact->GetFixtureB()->GetBody()->GetUserData();
	}

	float PhysicalContactIterator::getNativeBodyBPositionX()
	{
		return static_cast<PhysicalBody*>(contactEdge->contact->GetFixtureB()->GetBody()->GetUserData())->getX();
	}

	float PhysicalContactIterator::getNativeBodyBPositionY()
	{
		return static_cast<PhysicalBody*>(contactEdge->contact->GetFixtureB()->GetBody()->GetUserData())->getY();
	}

	IntPtr PhysicalContactIterator::getNativeFixtureB()
	{
		return contactEdge->contact->GetFixtureB()->GetUserData();
	}

	void PhysicalContactIterator::reset()
	{
		contactEdge = body->GetContactList();
	}

	bool PhysicalContactIterator::isEnd()
	{
		return contactEdge == nullptr;
	}

	void PhysicalContactIterator::next()
	{
		contactEdge = contactEdge->next;
	}

	PhysicalBody::PhysicalBody(b2Body* bodyInstance, PhysicalConverter dimensions, PhysicalConverter velocity): bodyIndex(-1)
	{
		body = bodyInstance;
		_dimensions = dimensions;
		_velocity = velocity;
		_contactsIterator.body = body;
	}

	PhysicalBody::~PhysicalBody()
	{		
	}

	void PhysicalBody::destroyFixtures()
	{
		for (uint i = 0; i < _fistures.size(); i++)
		{
			body->DestroyFixture(_fistures[i]->fixture);
			delete _fistures[i];
		}
		_fistures.clear();
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
		PhysicalFixture* fixture = new PhysicalFixture(fixtureInstance, _dimensions);
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

	void PhysicalBody::setVelocityX(float x)
	{
		b2Vec2 v = body->GetLinearVelocity();
		v.x = _velocity.toWorld(x);
		body->SetLinearVelocity(v);
		body->SetAwake(true);
	}

	void PhysicalBody::setVelocityY(float y)
	{
		b2Vec2 v = body->GetLinearVelocity();
		v.y = _velocity.toWorld(y);
		body->SetLinearVelocity(v);
		body->SetAwake(true);
	}

	float PhysicalBody::getX()
	{
		return _dimensions.fromWorld(body->GetPosition().x);
	}

	float PhysicalBody::getY()
	{
		return _dimensions.fromWorld(body->GetPosition().y);
	}

	void PhysicalBody::setX(float x)
	{
		b2Vec2 pos = body->GetPosition();
		pos.x = _dimensions.toWorld(x);
		body->SetTransform(pos, body->GetAngle());
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		body->SetAwake(true);
	}

	void PhysicalBody::setY(float y)
	{
		b2Vec2 pos = body->GetPosition();
		pos.y = _dimensions.toWorld(y);
		body->SetTransform(pos, body->GetAngle());
		body->SetAngularVelocity(0.0f);
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		body->SetAwake(true);
	}

	float PhysicalBody::getRotation()
	{
		return body->GetAngle();
	}

	void PhysicalBody::setRotation(float value)
	{
		body->SetTransform(body->GetPosition(), value);
	}

	llge::IPhysicalContactIterator* PhysicalBody::getContactIterator()
	{
		return &_contactsIterator;
	}

	llge::IPhysicalFixture* PhysicalBody::createEdgeFixture(float x0, float y0, float x1, float y1, llge::PhysicsFixtureConfig config)
	{
		PhysicalEdge edge;
		PhysicalFactory::initEdge(x0, y0, x1, y1, _dimensions, &edge);
		PhysicalFixture* fixture = createFixture(config, &edge);
		fixture->fixture->SetUserData(fixture);
		return fixture;
	}

	llge::IPhysicalFixture* PhysicalBody::createCircleFixture(float x, float y, float r, llge::PhysicsFixtureConfig config)
	{
		PhysicalCircle circle;
		PhysicalFactory::initCircle(x, y, r, _dimensions, &circle);
		PhysicalFixture* fixture = createFixture(config, &circle);
		fixture->fixture->SetUserData(fixture);
		return fixture;
	}

	llge::IPhysicalFixture* PhysicalBody::createBoxFixture(float x, float y, float rx, float ry, float rotation, llge::PhysicsFixtureConfig config)
	{
		PhysicalPolygon polygon;
		PhysicalFactory::initBox(x, y, rx, ry, rotation, _dimensions, &polygon);
		PhysicalFixture* fixture = createFixture(config, &polygon);
		fixture->fixture->SetUserData(fixture);
		return fixture;
	}

	llge::IPhysicalFixture* PhysicalBody::createPolygonFixture(IntPtr vertices2f, int count, llge::PhysicsFixtureConfig config)
	{
		PhysicalPolygon polygon;
		PhysicalFactory::initPolygon(static_cast<const core::Vector2 *>(vertices2f), count, _dimensions, &polygon);
		PhysicalFixture* fixture = createFixture(config, &polygon);
		fixture->fixture->SetUserData(fixture);
		return fixture;
	}
}