#include "PhysicalFactory.h"
#include "PhysicalWorld.h"

namespace physics
{
	PhysicalWorld* PhysicalFactory::createWorld()
	{
		return new PhysicalWorld(Dimensions, Velocity);
	}

	PhysicalFactory::PhysicalFactory(float scaleDimensions, float scaleVelocity)
	{
		Dimensions.setup(scaleDimensions);
		Velocity.setup(scaleVelocity);
	}

	PhysicalFactory::~PhysicalFactory()
	{
	}
}