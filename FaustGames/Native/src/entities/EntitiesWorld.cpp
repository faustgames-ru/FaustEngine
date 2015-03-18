#include "EntitiesWorld.h"

namespace entities
{
	EntitiesWorld::EntitiesWorld() :
		_filter(&_defaultFilter),
		_entitiesCounter(0)
	{

	}

	EntitiesWorld::~EntitiesWorld()
	{
	}

	Entity* EntitiesWorld::createEntity()
	{
		return new (core::Mem::alloc<Entity>()) Entity(++_entitiesCounter);
	}


	void EntitiesWorld::addUpdateSystem(System* system)
	{
		_updateSystems.push_back(system);
		_allSystems.push_back(system);
	}

	void EntitiesWorld::addRenderSystem(System* system)
	{
		_renderSystems.push_back(system);
		_allSystems.push_back(system);
	}

	void EntitiesWorld::addEntity(Entity* entity)
	{
		_filter->addEntity(entity);
		int count = _allSystems.size();
		for (int i = 0; i < count; i++)
		{
			addEntity(_allSystems[i], entity);
		}
	}

	void EntitiesWorld::removeEntity(Entity* entity)
	{
		_filter->removeEntity(entity);
		int count = _allSystems.size();
		for (int i = 0; i < count; i++)
		{
			removeEntity(_allSystems[i], entity);
		}
	}

	void EntitiesWorld::updateComponent(Entity* entity, ComponentMask::e type)
	{
		_filter->updateComponent(entity, type);
		int count = _allSystems.size();
		for (int i = 0; i < count; i++)
		{
			updateComponent(_allSystems[i], entity, type);
		}
	}

	void EntitiesWorld::updateWorld(float elapsed)
	{
		_filter->filtUpdate(_updateFilterResults);
		// todo: order by system components usage
		
		int count = _updateSystems.size();
		for (int i = 0; i < count; i++)
		{
			updateSystem(elapsed, _updateSystems[i], _updateFilterResults);
		}
	}

	void EntitiesWorld::renderWorld(float elapsed)
	{
		_filter->filtRender(_renderFilterResults);
		// todo: order by system components usage
		int count = _renderSystems.size();
		for (int i = 0; i < count; i++)
		{
			updateSystem(elapsed, _renderSystems[i], _renderFilterResults);
		}
	}

	void EntitiesWorld::setFilter(EntitiesFilter *filter)
	{
		_filter = filter;
	}


	void EntitiesWorld::addEntity(System* system, Entity* entity)
	{
		system->addEntity(entity);
	}

	void EntitiesWorld::removeEntity(System* system, Entity* entity)
	{
		system->removeEntity(entity);
	}
	
	void EntitiesWorld::updateComponent(System* system, Entity* entity, ComponentMask::e type)
	{
		system->updateComponent(entity, type);
	}


	void EntitiesWorld::updateSystem(float elapsed, System* system, const std::vector<Entity *> &filterResults)
	{
		// todo: order by system components usage & iterate ordered arrays
		system->updateSystem(elapsed, filterResults);		
	}

}