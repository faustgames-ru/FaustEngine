#ifndef DUMMY_ENTITIES_FILTER_H
#define DUMMY_ENTITIES_FILTER_H

#include "entities_classes.h"
#include "Entity.h"
#include "EntitiesFilter.h"

namespace entities
{
	class DummyEntitiesFilter : public EntitiesFilter
	{
	public:		
		virtual void addEntity(Entity* entity);
		virtual void removeEntity(Entity* entity);
		virtual void updateComponent(Entity* entity, ComponentMask::e type);
		virtual void filtUpdate(std::vector<Entity*> &result);
		virtual void filtRender(std::vector<Entity*> &result);
		virtual void clear();
	private:
		void filt(std::vector<Entity*> &result);
		EntitiesMap _entities;
	};
}

#endif /*DUMMY_ENTITIES_FILTER_H*/
