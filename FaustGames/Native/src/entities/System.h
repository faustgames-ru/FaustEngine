#ifndef SYSTEM_H
#define SYSTEM_H

#include "entities_classes.h"
#include "Entity.h"

namespace entities
{
	class System
	{
	public:
		virtual void addEntity(Entity *entity) {};
		virtual void removeEntity(Entity *entity) {};
		virtual void updateComponent(Entity *entity, ComponentMask::e type) {};
		virtual void updateSystem(float elapsed, const std::vector<Entity *> entities) = 0;
	private:
	};
}

#endif /*SYSTEM_H*/