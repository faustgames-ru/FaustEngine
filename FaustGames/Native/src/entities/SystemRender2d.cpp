#include "SystemRender2d.h"
#include "ComponentStaticMesh2d.h"

namespace entities
{
	RenderSystem2d::RenderSystem2d()
	{
	}

	RenderSystem2d::~RenderSystem2d()
	{
	}

	void RenderSystem2d::insert(int entityId, Entity *entity, ComponentAabb2d *component)
	{
	}
	
	void RenderSystem2d::update(int entityId, Entity *entity, ComponentAabb2d *component)
	{
	}
	
	void RenderSystem2d::remove(int entityId, Entity *entity, ComponentAabb2d *component)
	{
	}


	void RenderSystem2d::create()
	{
	}

	void RenderSystem2d::load()
	{
	}
	
	void RenderSystem2d::cleanup()
	{
		_quadTree.clear();
	}
	
	void RenderSystem2d::renderSystem()
	{
		_quadTree.filt(_visibleBounds, _visibleList);
		int count = _visibleList.items.count; 
		int entityId(0);
		//Entity *entity;
		for (int i = 0; i < count; i++)
		{
			int entityId = _visibleList.items.data[i];
			//entity = Entity::Pool.get(entityId);
			//ComponentAabb2d* aabb = entity->getComponent<ComponentAabb2d>();
			/// order by z layer
		}

	}


}