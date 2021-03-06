#include "TransformSystem2d.h"
#include "Transform2dComponent.h"

namespace entities
{
	void TransformSystem2d::addEntity(Entity *entity)
	{
		Transform2dComponent *component = entity->getComponent<Transform2dComponent>(); 
		if (!component) return;
	}
	
	void TransformSystem2d::removeEntity(Entity *entity)
	{
		Transform2dComponent *component = entity->getComponent<Transform2dComponent>();
		if (!component) return;
		component->Parent = 0;
	}
	
	void TransformSystem2d::updateComponent(Entity *entity, ComponentMask::e type)
	{
		if (type != ComponentMask::Transform2d) return;
		Transform2dComponent *component = entity->getComponent<Transform2dComponent>();
		if (!component) return;
		component->update();
	}
}