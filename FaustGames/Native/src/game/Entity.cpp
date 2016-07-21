#include "Entity.h"

namespace game
{
	void Entity::addComponent(Component* component)
	{
		components.push_back(component);
		component->owner = this;
		component->leaf = geometry::QuadTreeLeaf::create();
		component->leaf->userData = component;
	}
}