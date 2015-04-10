#include "DummyEntitiesFilter.h"

namespace entities
{
	void DummyEntitiesFilter::addEntity(Entity* entity)
	{
		_entities[entity->getId()] = entity;
	}
	
	void DummyEntitiesFilter::removeEntity(Entity* entity)
	{
		_entities.erase(entity->getId());
	}

	void DummyEntitiesFilter::updateComponent(Entity* entity, ComponentMask::e type)
	{
	}
		
	void DummyEntitiesFilter::filtUpdate(std::vector<Entity*> &result)
	{
		filt(result);
	}

	void DummyEntitiesFilter::filtRender(std::vector<Entity*> &result)
	{
		filt(result);
	}

	void DummyEntitiesFilter::clear()
	{
		_entities.clear();
	}


	void DummyEntitiesFilter::filt(std::vector<Entity*> &result)
	{
		result.clear();
		
		for (EntitiesMap::iterator i = _entities.begin(); i != _entities.end(); i++)
		{			
			result.push_back(i->second);
		}
	}

}