#ifndef PHYSICAL_WORLD_H
#define PHYSICAL_WORLD_H

#include "physics_classes.h"
#include "PhysicalConverter.h"
#include "PhysicalBody.h"
#include "PhysicalJoint.h"

namespace physics
{
	class PhysicalWorld : public llge::IPhysicalWorld
	{
	public:	
		PhysicalWorld(PhysicalConverter dimensions, PhysicalConverter velocity);
		~PhysicalWorld();

		void setGravity(float x, float y);
		PhysicalBody* createBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation);
		void destroyBody(PhysicalBody* body);

		PhysicalFixedJoint* createFixedJoint(PhysicalBody* body, float x, float y, float maxForce);
		void destroyJoint(PhysicalJoint* joint);

		virtual llge::IPhysicalBody* API_CALL createPhysicalBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation, IntPtr userData) OVERRIDE;
		virtual void API_CALL disposePhysicalBody(llge::IPhysicalBody* body) OVERRIDE;
		virtual llge::IPhysicalFixedJoint* API_CALL createPhysicalFixedJoint(llge::IPhysicalBody* body, float x, float y, float maxForce) OVERRIDE;
		virtual void API_CALL disposePhysicalJoint(llge::IPhysicalFixedJoint* joint) OVERRIDE;
		virtual void API_CALL step(float dt, int velocityIterations, int positionIterations) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
	private:
		b2World* _world;
		std::vector<PhysicalBody*> _bodies;
		std::vector<PhysicalJoint*> _joints;
		PhysicalConverter _dimensions;
		PhysicalConverter _velocity;
	};
}

#endif /*PHYSICAL_WORLD_H*/