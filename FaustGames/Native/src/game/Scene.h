#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "game_classes.h"
#include "../core/Delegate.h"
#include "../graphics/utilities/Camera.h"
#include "../geometry/QuadTree.h"
#include "../content/serialization/ContentObject.h"

namespace game
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		void enqueueResources(const LoadArgs& e);
		void loaded();
		void update();
		void invalidate(Entity *);
		void invalidate(Component *) const;
		geometry::Aabb bounds() const;
		graphics::Camera2d* camera();
	private:
		void enqueueEntityResources(const LoadArgs& e);
		geometry::Aabb loadAabb(content::ContentObject* value);
		void loadCamera(content::ContentObject* value);
		static const int ComponentsUpdateOrderSize = 256;
		void addLeaf(geometry::QuadTreeLeaf* leaf);
		graphics::Camera2d _camera;
		std::vector<Component* > _inputList[ComponentsUpdateOrderSize];
		std::vector<Component* > _updateList[ComponentsUpdateOrderSize];
		std::vector<Component* > _renderList[ComponentsUpdateOrderSize];
		std::vector<Entity* > _entities;
		geometry::QuadTree* _tree;
		geometry::Aabb _bounds;
	};
}

#endif /*GAME_SCENE_H*/