#ifndef ENTITIES_FILTER_H
#define ENTITIES_FILTER_H

#include "entities_classes.h"
#include "Entity.h"
#include "Component.h"

namespace entities
{
	class EntitiesFilter
	{
	public:
		virtual void addEntity(Entity* entity) = 0;
		virtual void removeEntity(Entity* entity) = 0;
		virtual void updateComponent(Entity* entity, ComponentMask::e type) = 0;
		virtual void filtUpdate(std::vector<Entity*> &result) = 0;
		virtual void filtRender(std::vector<Entity*> &result) = 0;
		virtual void clear() = 0;
	private:
	};
}

#endif /*ENTITIES_WORLD_H*/
