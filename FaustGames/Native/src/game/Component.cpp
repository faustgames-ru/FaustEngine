#include "Component.h"
#include "Entity.h"

namespace game
{
	Component::Component(): updateOrder(0), owner(nullptr), position(0.0f, 0.0f, 0.0f), halfSize(1.0f, 1.0f, 1.0f), leaf(nullptr)
	{
	}

	geometry::Aabb Component::getAabb() const
	{
		return geometry::Aabb::fromCenter(owner->position + position, halfSize);
	}
}
