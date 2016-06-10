#include "PhysicalWorld.h"
#include "../core/DebugRender.h"

namespace physics
{
	PhysicalWorld::PhysicalWorld(PhysicalConverter dimensions, PhysicalConverter velocity)
	{
		_world = new b2World(b2Vec2(0, -10.0f));
		_dimensions = dimensions;
		_velocity = velocity;
		_debugRenderCallback = new DebugRenderCallback(this, dimensions);
	}

	PhysicalWorld::~PhysicalWorld()
	{
		for (int i = 0; i < _joints.size(); i++)
		{
			_world->DestroyJoint(_joints[i]->joint);
			delete _joints[i];
		}
		_joints.clear();

		for (int i = 0; i < _bodies.size(); i++)
		{
			_world->DestroyBody(_bodies[i]->body);
			delete _bodies[i];
		}
		_bodies.clear();

		delete _debugRenderCallback;
		delete _world;
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

	PhysicalFixedJoint* PhysicalWorld::createFixedJoint(PhysicalBody* groundBody, PhysicalBody* body, float x, float y, float maxForce)
	{
		b2MouseJointDef jointDef;
		jointDef.maxForce = maxForce;
		jointDef.target = b2Vec2(_dimensions.toWorld(x), _dimensions.toWorld(y));
		jointDef.bodyA = groundBody->body;
		jointDef.bodyB = body->body;
		b2MouseJoint* joint = static_cast<b2MouseJoint*>(_world->CreateJoint(&jointDef));

		PhysicalFixedJoint* result = new PhysicalFixedJoint(joint, _dimensions);
		result->index = _joints.size();
		_joints.push_back(result);
		return result;
	}

	void PhysicalWorld::destroyJoint(PhysicalJoint* joint)
	{
		int index = joint->index;
		_world->DestroyJoint(_joints[index]->joint);
		_joints[index] = _joints.back();
		_joints[index]->index = index;
		_joints.pop_back();
		delete joint;
	}

	uint PhysicalWorld::getDebugFixtureColor(b2Fixture* fixture)
	{
		if (fixture->IsSensor())
			return 0xffff00ff;
		return 0xffffffff;
	}

	void PhysicalWorld::debugRender(float x, float y, float rx, float ry)
	{
		b2AABB aabb;
		aabb.lowerBound = b2Vec2(_dimensions.toWorld(x-rx), _dimensions.toWorld(y-ry));
		aabb.upperBound = b2Vec2(_dimensions.toWorld(x+rx), _dimensions.toWorld(y+ry));
		_world->QueryAABB(_debugRenderCallback, aabb);
	}

	void PhysicalWorld::debugRender()
	{
		b2Body* b = _world->GetBodyList();
		for (; b != nullptr;)
		{
			debugRenderBody(b);
			b = b->GetNext();
		}
	}

	void PhysicalWorld::debugRenderBody(b2Body* body)
	{
		b2Fixture* f = body->GetFixtureList();
		core::Vector2 center;
		for (; f != nullptr;)
		{
			center.setX(_dimensions.fromWorld(body->GetPosition().x));
			center.setY(_dimensions.fromWorld(body->GetPosition().y));
			debugRenderFixture(center, f);
			f = f->GetNext();
		}
	}

	void PhysicalWorld::debugRenderFixture(core::Vector2 center, b2Fixture* fixture)
	{
		core::DebugDraw *draw = &core::DebugDraw::Default;
		b2Shape* shape = fixture->GetShape();
		uint color = getDebugFixtureColor(fixture);

		b2CircleShape *circle;
		b2PolygonShape *polygon;
		core::Vector3 a, b;
		uint count;
		switch (shape->GetType())
		{
		case b2Shape::e_circle: 
			circle = dynamic_cast<b2CircleShape *>(shape);
			a.setX(center.getX() + _dimensions.fromWorld(circle->m_p.x));
			a.setY(center.getY() + _dimensions.fromWorld(circle->m_p.y));
			a.setZ(0);
			draw->circle(color, a, _dimensions.fromWorld(circle->m_radius));
			break;
		case b2Shape::e_polygon: 
			polygon = dynamic_cast<b2PolygonShape *>(shape);			
			count = polygon->GetVertexCount();
			for (uint i = 0, j = count-1; i< count; ++i)
			{
				a.setX(center.getX() + _dimensions.fromWorld(polygon->GetVertex(j).x));
				a.setY(center.getY() + _dimensions.fromWorld(polygon->GetVertex(j).y));
				a.setZ(0);				
				b.setX(center.getX() + _dimensions.fromWorld(polygon->GetVertex(i).x));
				b.setY(center.getY() + _dimensions.fromWorld(polygon->GetVertex(i).y));
				b.setZ(0);
				draw->edge(color, a, b);
				j = i;
			}
			break;
		default: 
			break;
		}
	}

	llge::IPhysicalBody* PhysicalWorld::createPhysicalBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation, IntPtr userData)
	{
		PhysicalBody* body = createBody(type, x, y, rotation, fixedRotation);
		body->body->SetUserData(userData);
		return body;
	}

	void PhysicalWorld::disposePhysicalBody(llge::IPhysicalBody* body)
	{
		destroyBody(static_cast<PhysicalBody *>(body->getNativeInstance()));
	}

	llge::IPhysicalFixedJoint* PhysicalWorld::createPhysicalFixedJoint(llge::IPhysicalBody* ground, llge::IPhysicalBody* body, float x, float y, float maxForce)
	{
		return createFixedJoint(static_cast<PhysicalBody *>(ground->getNativeInstance()), static_cast<PhysicalBody *>(body->getNativeInstance()), x, y, maxForce);
	}

	void PhysicalWorld::disposePhysicalJoint(llge::IPhysicalFixedJoint* joint)
	{
		destroyJoint(static_cast<PhysicalFixedJoint *>(joint->getNativeInstance()));
	}

	void PhysicalWorld::step(float dt, int velocityIterations, int positionIterations)
	{
		_world->Step(dt, velocityIterations, positionIterations); // 8, 3
	}

	void PhysicalWorld::dispose()
	{
		delete this;
	}

	DebugRenderCallback::DebugRenderCallback(PhysicalWorld* world, PhysicalConverter dimensions)
	{
		_world = world;
		_dimensions = dimensions;
	}

	bool DebugRenderCallback::ReportFixture(b2Fixture* fixture)
	{
		core::Vector2 center;
		b2Body* body = fixture->GetBody();
		center.setX(_dimensions.fromWorld(body->GetPosition().x));
		center.setY(_dimensions.fromWorld(body->GetPosition().y));
		_world->debugRenderFixture(center, fixture);
		return true;
	}
}