#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "game_classes.h"
#include "Entity.h"
#include "../graphics/utilities/Camera.h"

namespace game
{
	class Scene
	{
	public:
		void update();
		void invalidate(Entity *);
	private:
		static const int ComponentsUpdateOrderSize = 256;
		void addLeaf(geometry::QuadTreeLeaf* leaf);
		graphics::Camera2d _camera;
		std::vector<Component* > _updateList[ComponentsUpdateOrderSize];
		std::vector<Entity* > _entities;
		geometry::QuadTree* _tree;
	};
}

#endif /*GAME_SCENE_H*/