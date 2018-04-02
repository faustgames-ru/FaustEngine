#include "PhysicalWorld.h"
#include "PhysicalFixture.h"
#include "../core/DebugRender.h"

namespace physics
{
	PhysicalWorld::PhysicalWorld(PhysicalConverter dimensions, PhysicalConverter velocity)
	{
		_world = new b2World(b2Vec2(0, -10.0f));
		_dimensions = dimensions;
		_velocity = velocity;
		_debugRenderCallback = new DebugRenderCallback(this, dimensions);
		_world->SetContactListener(&_platformsContactListener);
	}

	PhysicalWorld::~PhysicalWorld()
	{
		for (uint i = 0; i < _joints.size(); i++)
		{
			// joints will be romoved by their bodies
			//_world->DestroyJoint(_joints[i]->joint);
			delete _joints[i];
		}
		_joints.clear();

		for (uint i = 0; i < _bodies.size(); i++)
		{
			_bodies[i]->destroyFixtures();
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
		body->destroyFixtures();
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

	bool PhysicalWorld::makeRayCastFirstEx(float x0, float y0, float x1, float y1, uint raycastMask, uint mask, bool ignoreSensors, IntPtr result)
	{
		bool res = rayCastFirst(x0, y0, x1, y1, raycastMask, mask, ignoreSensors, *static_cast<llge::RayCastResult*>(result));
		return res;
	}

	bool PhysicalWorld::makeRayCastFirst(float x0, float y0, float x1, float y1, uint raycastMask, uint mask, bool ignoreSensors, IntPtr resultPoint, IntPtr resultNormal)
	{
		bool result = rayCastFirst(x0, y0, x1, y1, raycastMask, mask, ignoreSensors, *static_cast<core::Vector2 *>(resultPoint), *static_cast<core::Vector2 *>(resultNormal));
		return result;
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
			float sin = core::Math::sin(body->GetAngle());
			float cos = core::Math::cos(body->GetAngle());
			debugRenderFixture(center, sin, cos, f);
			f = f->GetNext();
		}
	}

	void PhysicalWorld::debugRenderFixture(core::Vector2 center, float sin, float cos, b2Fixture* fixture)
	{
		core::DebugDraw *draw = &core::DebugDraw::Default;
		b2Shape* shape = fixture->GetShape();
		uint color = getDebugFixtureColor(fixture);
		debugRenderShape(color, center, sin, cos, shape);
	}

	void PhysicalWorld::debugRenderShape(uint color, core::Vector2 center, float sin, float cos, b2Shape* shape)
	{
		core::DebugDraw *draw = &core::DebugDraw::Default;

		b2CircleShape *circle;
		b2EdgeShape *edge;
		b2PolygonShape *polygon;
		core::Vector3 a, b;
		core::Vector2 v0;
		core::Vector2 v1;
		uint count;
		switch (shape->GetType())
		{
		case b2Shape::e_edge:
			// todo: rotation support
			edge = static_cast<b2EdgeShape*>(shape);
			a.setX(center.getX() + _dimensions.fromWorld(edge->m_vertex1.x));
			a.setY(center.getY() + _dimensions.fromWorld(edge->m_vertex1.y));
			a.setZ(0);
			b.setX(center.getX() + _dimensions.fromWorld(edge->m_vertex2.x));
			b.setY(center.getY() + _dimensions.fromWorld(edge->m_vertex2.y));
			b.setZ(0);
			draw->edge(color, a, b);
			if (!edge->m_hasVertex0)
			{
				core::Vector2 n = (core::Vector2(edge->m_vertex1.x - edge->m_vertex2.x, edge->m_vertex1.y - edge->m_vertex2.y)).normalize().rotate90cw();
				a.setX(center.getX() + _dimensions.fromWorld(edge->m_vertex1.x) + n.getX() * 5.0f);
				a.setY(center.getY() + _dimensions.fromWorld(edge->m_vertex1.y) + n.getY() * 5.0f);
				a.setZ(0);
				b.setX(center.getX() + _dimensions.fromWorld(edge->m_vertex1.x) - n.getX() * 5.0f);
				b.setY(center.getY() + _dimensions.fromWorld(edge->m_vertex1.y) - n.getY() * 5.0f);
				b.setZ(0);
				draw->edge(color, a, b);
			}
			if (!edge->m_hasVertex3)
			{
				core::Vector2 n = (core::Vector2(edge->m_vertex1.x - edge->m_vertex2.x, edge->m_vertex1.y - edge->m_vertex2.y)).normalize().rotate90cw();				
				a.setX(center.getX() + _dimensions.fromWorld(edge->m_vertex2.x) + n.getX() * 5.0f);
				a.setY(center.getY() + _dimensions.fromWorld(edge->m_vertex2.y) + n.getY() * 5.0f);
				a.setZ(0);
				b.setX(center.getX() + _dimensions.fromWorld(edge->m_vertex2.x) - n.getX() * 5.0f);
				b.setY(center.getY() + _dimensions.fromWorld(edge->m_vertex2.y) - n.getY() * 5.0f);
				b.setZ(0);
				draw->edge(color, a, b);
			}
			break;
		case b2Shape::e_circle:
			circle = static_cast<b2CircleShape *>(shape);
			v0 = core::Vector2(_dimensions.fromWorld(circle->m_p.x), _dimensions.fromWorld(circle->m_p.y));
			v0 = v0.rotate(cos, sin);

			a.setX(center.getX() + v0.getX());
			a.setY(center.getY() + v0.getY());
			a.setZ(0);
			draw->circle(color, a, _dimensions.fromWorld(circle->m_radius));
			break;
		case b2Shape::e_polygon:
			polygon = static_cast<b2PolygonShape *>(shape);
			count = polygon->GetVertexCount();
			for (uint i = 0, j = count - 1; i< count; ++i)
			{
				v0 = core::Vector2(_dimensions.fromWorld(polygon->GetVertex(j).x), _dimensions.fromWorld(polygon->GetVertex(j).y));
				v1 = core::Vector2(_dimensions.fromWorld(polygon->GetVertex(i).x), _dimensions.fromWorld(polygon->GetVertex(i).y));				
				v0 = v0.rotate(cos, sin);
				v1 = v1.rotate(cos, sin);
				a.setX(center.getX() + v0.getX());
				a.setY(center.getY() + v0.getY());
				a.setZ(0);
				b.setX(center.getX() + v1.getX());
				b.setY(center.getY() + v1.getY());
				b.setZ(0);
				draw->edge(color, a, b);
				j = i;
			}
			break;
		default:
			break;
		}
	}

	bool PhysicalWorld::rayCastFirst(float x0, float y0, float x1, float y1, ushort raycastMask, uint mask, bool ignoreSensors, llge::RayCastResult& result)
	{
		_raycastFirst.init(mask, ignoreSensors, raycastMask);
		_world->RayCast(&_raycastFirst, b2Vec2(_dimensions.toWorld(x0), _dimensions.toWorld(y0)), b2Vec2(_dimensions.toWorld(x1), _dimensions.toWorld(y1)));
		core::Vector2 resultPosition = core::Vector2(_dimensions.fromWorld(_raycastFirst.bestPoint.getX()), _dimensions.fromWorld(_raycastFirst.bestPoint.getY()));
		core::Vector2 resultNormal = _raycastFirst.bestNormal;

		result.resultNormalX = resultNormal.getX();
		result.resultNormalY = resultNormal.getY();
		result.resultPositionX = resultPosition.getX();
		result.resultPositionY = resultPosition.getY();
		result.fixture = nullptr;
		result.body = nullptr;
		bool hasRaycast = _raycastFirst.best != nullptr;
		if(hasRaycast)
		{
			PhysicalFixture *f = static_cast<PhysicalFixture *>(_raycastFirst.best->GetUserData());
			result.resultColisionGroup = f->getCollisionGroup();			
			result.resultRaycastGroup = f->getRaycastGroup();
			result.fixture = static_cast<llge::IPhysicalFixture*>(f);
			result.body = static_cast<llge::IPhysicalBody*>(f->fixture->GetBody()->GetUserData());
		}
		return hasRaycast;
	}

	bool PhysicalWorld::rayCastFirst(float x0, float y0, float x1, float y1, ushort raycastMask, uint mask, bool ignoreSensors, core::Vector2& result, core::Vector2 &resultNormal)
	{
		_raycastFirst.init(mask, ignoreSensors, raycastMask);
		_world->RayCast(&_raycastFirst, b2Vec2(_dimensions.toWorld(x0), _dimensions.toWorld(y0)), b2Vec2(_dimensions.toWorld(x1), _dimensions.toWorld(y1)));
		result = core::Vector2(_dimensions.fromWorld(_raycastFirst.bestPoint.getX()), _dimensions.fromWorld(_raycastFirst.bestPoint.getY()));
		resultNormal = _raycastFirst.bestNormal;
		bool hasRaycast = _raycastFirst.best != nullptr;
		return hasRaycast;
	}

	llge::IPhysicalBody* PhysicalWorld::createPhysicalBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation)
	{
		PhysicalBody* body = createBody(type, x, y, rotation, fixedRotation);
		body->body->SetUserData(body);
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
		float sin = core::Math::sin(body->GetAngle());
		float cos = core::Math::cos(body->GetAngle());
		_world->debugRenderFixture(center, sin, cos, fixture);
		return true;
	}

	RayCastFirstCallback::RayCastFirstCallback(): mask(0xffffffff), raycastMask(0xffff), best(nullptr), bestFraction(1.0f), ignoreSensors(true)
	{
	}

	void RayCastFirstCallback::init(uint maskBits, bool sensors, ushort raycastMaskBits)
	{
		ignoreSensors = sensors;
		bestFraction = 1.0f;
		best = nullptr;
		mask = maskBits;
		raycastMask = raycastMaskBits;
	}

	float RayCastFirstCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		if ((fixture->GetFilterData().categoryBits & mask) == 0)
			return 1.0f;
		if (ignoreSensors && fixture->IsSensor())
			return 1.0f;
		PhysicalFixture* f = static_cast<PhysicalFixture*>(fixture->GetUserData());
		ushort raycastGroup = f->getRaycastGroup();
		if ((raycastGroup & raycastMask) == 0)
			return 1.0f;
		if (fraction <= bestFraction)
		{
			best = fixture;
			bestFraction = fraction;
			bestPoint = core::Vector2(point.x, point.y);
			bestNormal = core::Vector2(normal.x, normal.y);
		}
		return 1.0f;
	}

	PlatformsContactListener::PlatformsContactListener(): platformsCategory(0x40)
	{
	}

	void PlatformsContactListener::BeginContact(b2Contact* contact)
	{		

		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		PhysicalFixture *physicalFixtureA = static_cast<PhysicalFixture *>(fixtureA->GetUserData());
		PhysicalFixture *physicalFixtureB = static_cast<PhysicalFixture *>(fixtureB->GetUserData());

		physicalFixtureA->contactsCounter++;
		physicalFixtureB->contactsCounter++;

		//check if one of the fixtures is the platform
		b2Fixture* platformFixture = nullptr;
		b2Fixture* otherFixture = nullptr;

		if (fixtureA->GetFilterData().categoryBits == platformsCategory) {
			platformFixture = fixtureA;
			otherFixture = fixtureB;
		}
		else if (fixtureB->GetFilterData().categoryBits == platformsCategory) {
			platformFixture = fixtureB;
			otherFixture = fixtureA;
		}

		if (!platformFixture)
			return;

		b2Body* platformBody = platformFixture->GetBody();
		b2Body* otherBody = otherFixture->GetBody();

		int numPoints = contact->GetManifold()->pointCount;
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);		
		
		/*
		if (core::Math::abs(worldManifold.normal.x * 2.0f) > core::Math::abs(worldManifold.normal.y))
			return;
		*/

		//check if contact points are moving downward
		for (int i = 0; i < numPoints; i++) {
			b2Vec2 pointVel =
				otherBody->GetLinearVelocityFromWorldPoint(worldManifold.points[i]);
			if (pointVel.y < -0.5f)
				return;//point is moving down, leave contact solid and exit
		}

		//no points are moving downward, contact should not be solid
		contact->SetEnabled(false);		
	}

	void PlatformsContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		PhysicalFixture *physicalFixtureA = static_cast<PhysicalFixture *>(fixtureA->GetUserData());
		PhysicalFixture *physicalFixtureB = static_cast<PhysicalFixture *>(fixtureB->GetUserData());
		physicalFixtureA->contactsCounter--;
		physicalFixtureB->contactsCounter--;

		contact->SetEnabled(true);
	}
}