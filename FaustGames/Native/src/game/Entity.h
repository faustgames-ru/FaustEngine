#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "game_classes.h"
#include "../geometry/Quadtree.h"

namespace game
{
	class Entity
	{
	public:
		std::vector<Component* > components;
		core::Vector3 position;
	private:
	};
}

#endif /*GAME_SCENE_H*/