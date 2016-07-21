#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include "game_classes.h"
#include "../content/serialization/ContentValue.h"
#include "../geometry/Aabb.h"
#include "../geometry/Quadtree.h"
#include "ComponentsFactory.h"

namespace game
{
	class Component: IBaseObject
	{
	public:
		Component();
		byte updateOrder;
		byte renderOrder;
		Entity* owner;
		core::Vector3 position;
		core::Vector3 halfSize;

		geometry::QuadTreeLeaf* leaf;
		geometry::Aabb getAabb() const;		
		float getZOrder() const;
		int getTypeId() const;
		void* getInstance() const;
		virtual void enqueueResources(const LoadArgs& e) {};
		virtual void loaded() {};
		virtual void update(const UpdateArgs& e) {};
		virtual void render() {};
		virtual void dispose();
	private:
		friend ComponentsFactory;
		int _typeId;
		void* _instance;
	};
}


#endif /*GAME_COMPONENT_H*/