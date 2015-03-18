#ifndef QUADTREE_ENTITIES_FILTER_H
#define QUADTREE_ENTITIES_FILTER_H

#include "entities_classes.h"
#include "Entity.h"
#include "EntitiesFilter.h"

namespace entities
{
	class QuadtreeEntitiesFilter : public EntitiesFilter
	{
	public:
		QuadtreeEntitiesFilter();
		~QuadtreeEntitiesFilter();
		void setUpdateAabb(const geometry::Aabb2d &aabb);
		void setRenderAabb(const geometry::Aabb2d &aabb);
		virtual void addEntity(Entity* entity);
		virtual void removeEntity(Entity* entity);
		virtual void updateComponent(Entity* entity, ComponentMask::e type);
		virtual void filtUpdate(std::vector<Entity*> &result);
		virtual void filtRender(std::vector<Entity*> &result);
	private:
		void filt(const geometry::Aabb2d &aabb, geometry::FilterResult filterResults, std::vector<Entity*> &result);
		geometry::Quadtree2d _quadTree;
		geometry::Aabb2d _updateAabb;
		geometry::Aabb2d _renderAabb;
		geometry::FilterResult _updateResults;
		geometry::FilterResult _renderResults;
		EntitiesMap _entities;
	};
}

#endif /*QUADTREE_ENTITIES_FILTER_H*/
