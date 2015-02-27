#ifndef ENTITIES_WORLD_H
#define ENTITIES_WORLD_H

#include "entities_classes.h"
#include "Component.h"
#include "Entity.h"
#include "System.h"
#include "DummyEntitiesFilter.h"

namespace entities
{
	class EntitiesWorld
	{
	public:
		EntitiesWorld();
		~EntitiesWorld();
		Entity* createEntity();
		void addUpdateSystem(System* system);
		void addRenderSystem(System* system);
		void addEntity(Entity* entity);
		void removeEntity(Entity* entity);
		void updateComponent(Entity* entity, ComponentType::e type);
		void updateWorld(float elapsed);
		void renderWorld(float elapsed);
		void setFilter(EntitiesFilter *filter);
	private:
		void addEntity(System* system, Entity* entity);
		void removeEntity(System* system, Entity* entity);
		void updateComponent(System* system, Entity* entity, ComponentType::e type);
		
		void updateSystem(float elapsed, System* system, const std::vector<Entity *> &filterResults);
		DummyEntitiesFilter _defaultFilter;
		EntitiesFilter *_filter;
		std::vector<Entity *> _updateFilterResults;
		std::vector<Entity *> _renderFilterResults;
		std::vector<System *> _updateSystems;
		std::vector<System *> _renderSystems;
		std::vector<System *> _allSystems;

		int _entitiesCounter;
	};
}

#endif /*ENTITIES_WORLD_H*/
