#ifndef TRANSFORM_SYSTEM_2D_H
#define TRANSFORM_SYSTEM_2D_H

#include "entities_classes.h"
#include "System.h"

namespace entities
{
	class TransformSystem2d : public System
	{
	public:
		virtual void addEntity(Entity *entity);
		virtual void removeEntity(Entity *entity);
		virtual void updateComponent(Entity *entity, ComponentMask::e type);
		virtual void updateSystem(float elapsed, const std::vector<Entity *> entities){}
	private:
	};
}

#endif /*TRANSFORM_SYSTEM_2D_H*/