#include "PhysicalWorld.h"

namespace physics
{
	PhysicalWorld::PhysicalWorld(PhysicalConverter dimensions, PhysicalConverter velocity)
	{
		_world = new b2World(b2Vec2(0, -10.0f));
		_dimensions = dimensions;
		_velocity = velocity;
	}

	PhysicalWorld::~PhysicalWorld()
	{
		for (int i = 0; i < _bodies.size(); i++)
		{
			_world->DestroyBody(_bodies[i]->body);
			delete _bodies[i];
		}
		_bodies.clear();
		delete _world;
	}

	void PhysicalWorld::dispose()
	{
		delete this;
	}

	void PhysicalWorld::setGravity(float x, float y)
	{
		_world->SetGravity(b2Vec2(x, y));
	}

	b2BodyType convertBodyType(llge::PhysicalBodyType type)
	{
		switch (type)
		{
		case llge::Static:
			return b2_staticBody;
		case llge::Dynamic:
			return b2_dynamicBody;
		case llge::Kinematic:
			return b2_kinematicBody;
		default:
			return b2_staticBody;
		}
	}

	PhysicalBody* PhysicalWorld::createBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation)
	{
		b2BodyDef myBodyDef;
		myBodyDef.type = convertBodyType(type);
		myBodyDef.position.Set(_dimensions.toWorld(x), _dimensions.toWorld(y)); //set the starting position
		myBodyDef.angle = rotation; 
		myBodyDef.fixedRotation = fixedRotation;
		
		b2Body* body = _world->CreateBody(&myBodyDef);
		PhysicalBody* result = new PhysicalBody(body, _dimensions, _velocity);
		result->bodyIndex = _bodies.size();
		_bodies.push_back(result);
		return result;
	}

	void PhysicalWorld::destroyBody(PhysicalBody* body)
	{
		int index = body->bodyIndex;
		_world->DestroyBody(_bodies[index]->body);
		_bodies[index] = _bodies.back();
		_bodies[index]->bodyIndex = index;
		_bodies.pop_back();
		delete body;
	}
}