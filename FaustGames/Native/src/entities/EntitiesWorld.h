#ifndef ENTITIES_WORLD_H
#define ENTITIES_WORLD_H

#include "entities_classes.h"
#include "Entity.h"
#include "EntitySystem.h"

namespace entities
{	
	class EntitiesWorld
	{
	public:
		static void addUpdateSystem(UpdateSystem *system);
		static void addRenderSystem(RenderSystem *system);
		static void insert(int entityId);
		static void update(int entityId, int staticComponentIndex);
		static void remove(int entityId);

		static void create();
		static void load();
		static void update(float time);
		static void render(float time);
		static void cleanup();
	protected:
	private:
		static std::vector<EntitySystem *> _systems;
		static std::vector<UpdateSystem *> _updateSystems;
		static std::vector<RenderSystem *> _renderSystems;
	};
}

#endif /*ENTITIES_WORLD_H*/

