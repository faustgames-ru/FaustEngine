#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include "game_classes.h"
#include "../core/AsyncState.h"
#include "../geometry/Aabb.h"
#include "../geometry/Quadtree.h"

namespace game
{
	class Component: IBaseObject
	{
	public:
		Component();
		byte updateOrder;
		Entity* owner;
		core::Vector3 position;
		core::Vector3 halfSize;

		geometry::QuadTreeLeaf* leaf;
		geometry::Aabb getAabb() const;
		virtual core::AsyncChain* load() = 0;
		virtual void loaded() = 0;
		virtual void update(const UpdateArgs& e) = 0;
	private:
	};
}


#endif