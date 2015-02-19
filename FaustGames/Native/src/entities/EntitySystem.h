#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include "entities_classes.h"
#include "Entity.h"

namespace entities
{
	class EntitySystem
	{
	public:
		inline unsigned int getStaticComponentsMask(){ return StaticComponentsMask; }
		unsigned int StaticComponentsMask;
		virtual void insert(int entityId) = 0;
		virtual void update(int entityId, int staticComponentIndex) = 0;
		virtual void remove(int entityId) = 0;

		virtual void create() = 0;
		virtual void load() = 0;
		virtual void cleanup() = 0;
	};

	class RenderSystem : public EntitySystem
	{
	public:
		virtual void renderSystem(float time) = 0;
	};

	template<typename T>
	class RenderSystemTemplate : public RenderSystem
	{
	public:
		RenderSystemTemplate()
		{
			StaticComponentsMask = T::Mask;
		}
		
		virtual void insert(int entityId, Entity *entity, T *component) = 0;
		virtual void update(int entityId, Entity *entity, T *component) = 0;
		virtual void remove(int entityId, Entity *entity, T *component) = 0;


		virtual void insert(int entityId)
		{
		}
		
		virtual void update(int entityId, int staticComponentIndex)
		{
		}
		
		virtual void remove(int entityId)
		{
		}

		virtual void renderSystem(float time) = 0;
	};

	class UpdateSystem : public EntitySystem
	{
	public:
		virtual void updateSystem(float time) = 0;
	};
}

#endif /*ENTITY_SYSTEM_H*/

