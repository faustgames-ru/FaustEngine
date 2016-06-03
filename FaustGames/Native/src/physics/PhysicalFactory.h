#ifndef PHYSICAL_FACTORY_H
#define PHYSICAL_FACTORY_H

#include "physics_classes.h"
#include "PhysicalConverter.h"

namespace physics
{
	class PhysicalFactory : public llge::IPhysicalFactory
	{
	public:
		PhysicalConverter Dimensions;
		PhysicalConverter Velocity;

		PhysicalWorld* createWorld();
		PhysicalFactory(float scaleDimensions, float scaleVelocity);
		~PhysicalFactory();
	};
}

#endif /*PHYSICAL_FACTORY_H*/