#include "QuadtreeEntitiesFilter.h"
#include "Aabb2dComponent.h"

namespace entities
{
	QuadtreeEntitiesFilter::QuadtreeEntitiesFilter()
	{
	}
	
	QuadtreeEntitiesFilter::~QuadtreeEntitiesFilter()
	{
	}
	
	void QuadtreeEntitiesFilter::setUpdateAabb(const geometry::Aabb2d &aabb)
	{
		_updateAabb = aabb;
	}
	
	void QuadtreeEntitiesFilter::setRenderAabb(const geometry::Aabb2d &aabb)
	{
		_renderAabb = aabb;
	}
	
	void QuadtreeEntitiesFilter::addEntity(Entity* entity)
	{
		Aadd2dComponent *aabb = entity->getComponent<Aadd2dComponent>();
		if (aabb)
		{
			_quadTree.insert(aabb->Aadd, entity->getId());
			_entities[entity->getId()] = entity;
		}
	}
	
	void QuadtreeEntitiesFilter::removeEntity(Entity* entity)
	{
		_quadTree.remove(entity->getId());
		_entities.erase(entity->getId());
	}
	
	void QuadtreeEntitiesFilter::updateComponent(Entity* entity, ComponentMask::e type)
	{
		if (!(type & ComponentMask::Aadd2d)) return;
		Aadd2dComponent *aabb = entity->getComponent<Aadd2dComponent>();
		if (aabb)
		{
			_quadTree.remove(entity->getId());
			_quadTree.insert(aabb->Aadd, entity->getId());
			_entities[entity->getId()] = entity;
		}
	}
	
	void QuadtreeEntitiesFilter::filtUpdate(std::vector<Entity*> &result)
	{
		filt(_updateAabb, _updateResults, result);
	}
	
	void QuadtreeEntitiesFilter::filtRender(std::vector<Entity*> &result)
	{
		filt(_renderAabb, _renderResults, result);
	}

	void QuadtreeEntitiesFilter::clear()
	{
		_quadTree.clear();
	}

	
	void QuadtreeEntitiesFilter::filt(const geometry::Aabb2d &aabb, geometry::FilterResult filterResults, std::vector<Entity*> &result)
	{
		result.clear();
		filterResults.items.count = 0;
		_quadTree.filt(aabb, filterResults);
		int count = filterResults.items.count;
		for (int i = 0; i < count; i++)
			result.push_back(_entities[filterResults.items.data[i]]);
	}

}