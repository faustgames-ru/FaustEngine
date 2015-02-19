#ifndef SYSTEM_RENDER_2D_H
#define SYSTEM_RENDER_2D_H

#include "entities_classes.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "ComponentAabb2d.h"

namespace entities
{	
	class RenderSystem2d : public RenderSystemTemplate<ComponentAabb2d>
	{
	public:
		RenderSystem2d();
		~RenderSystem2d();

		virtual void insert(int entityId, Entity *entity, ComponentAabb2d *component);
		virtual void update(int entityId, Entity *entity, ComponentAabb2d *component);
		virtual void remove(int entityId, Entity *entity, ComponentAabb2d *component);
		
		virtual void create();
		virtual void load();
		virtual void cleanup();
		virtual void renderSystem();
	private:
		graphics::GraphicsDevice _graphicsDevice;
		geometry::Quadtree2d _quadTree;
		geometry::Aabb2d _visibleBounds;
		geometry::FilterResult _visibleList;
	};
}

#endif /*SYSTEM_RENDER_2D_H*/
