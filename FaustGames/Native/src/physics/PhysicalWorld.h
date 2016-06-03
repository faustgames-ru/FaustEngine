#ifndef PHYSICAL_WORLD_H
#define PHYSICAL_WORLD_H

#include "physics_classes.h"
#include "PhysicalConverter.h"
#include "PhysicalBody.h"

namespace physics
{
	class PhysicalWorld : public llge::IPhysicalWorld
	{
	public:	
		PhysicalWorld(PhysicalConverter dimensions, PhysicalConverter velocity);
		~PhysicalWorld();

		virtual void API_CALL dispose() OVERRIDE;

		void setGravity(float x, float y);
		PhysicalBody* createBody(llge::PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation);
		void destroyBody(PhysicalBody* body);
	private:
		b2World* _world;
		std::vector<PhysicalBody*> _bodies;
		PhysicalConverter _dimensions;
		PhysicalConverter _velocity;
	};
}

#endif /*PHYSICAL_WORLD_H*/