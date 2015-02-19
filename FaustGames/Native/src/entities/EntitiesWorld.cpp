#include "EntitiesWorld.h"

namespace entities
{
	void EntitiesWorld::addUpdateSystem(UpdateSystem *system)
	{
		_updateSystems.push_back(system);
		_systems.push_back(system);
	}

	void EntitiesWorld::addRenderSystem(RenderSystem *system)
	{
		_renderSystems.push_back(system);
		_systems.push_back(system);
	}

	void EntitiesWorld::insert(int entityId)
	{
	}

	void EntitiesWorld::update(int entityId, int ComponentsMask)
	{
	}

	void EntitiesWorld::remove(int entityId)
	{
	}

	void EntitiesWorld::create()
	{
	}

	void EntitiesWorld::load()
	{
	}
	
	void EntitiesWorld::update(float time)
	{
	}
	
	void EntitiesWorld::render(float time)
	{
	}
	
	void EntitiesWorld::cleanup()
	{
	}

	std::vector<EntitySystem *> EntitiesWorld::_systems;
	std::vector<UpdateSystem *> EntitiesWorld::_updateSystems;
	std::vector<RenderSystem *> EntitiesWorld::_renderSystems;


}