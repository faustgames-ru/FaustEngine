#include "Component.h"
#include "Entity.h"

namespace game
{
	Component::Component(): inputOrder(InputOrder::None), updateOrder(UpdateOrder::None), renderOrder(RenderOrder::None), owner(nullptr), position(0.0f, 0.0f, 0.0f), halfSize(1.0f, 1.0f, 1.0f), leaf(nullptr), _typeId(-1), _instance(nullptr)
	{
	}

	geometry::Aabb Component::getAabb() const
	{
		return geometry::Aabb::fromCenter(owner->position + position, halfSize);
	}

	float Component::getZOrder() const
	{
		return owner->position.getZ() + position.getZ();
	}

	int Component::getTypeId() const
	{
		return _typeId;
	}

	void* Component::getInstance() const
	{
		return _instance;
	}

	InputStatus::e Component::input()
	{
		return InputStatus::None;
	}

	void Component::dispose()
	{
		if (leaf != nullptr)
			leaf->dispose();
		delete this;
	}
}
