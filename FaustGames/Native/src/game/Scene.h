#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "game_classes.h"
#include "../graphics/utilities/Camera.h"
#include "../geometry/QuadTree.h"
#include "../core/AsyncState.h"
#include "../content/serialization/ContentObject.h"

namespace game
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		core::AsyncChain* load(content::ContentObject* value);
		void update();
		void invalidate(Component *);
	private:
		core::AsyncChain* loadEntity(content::ContentObject* value);
		geometry::Aabb loadAabb(content::ContentObject* value);
		void loadCamera(content::ContentObject* value);
		static const int ComponentsUpdateOrderSize = 256;
		void addLeaf(geometry::QuadTreeLeaf* leaf);
		graphics::Camera2d _camera;
		std::vector<Component* > _updateList[ComponentsUpdateOrderSize];
		std::vector<Entity* > _entities;
		geometry::QuadTree* _tree;
	};
}

#endif /*GAME_SCENE_H*/