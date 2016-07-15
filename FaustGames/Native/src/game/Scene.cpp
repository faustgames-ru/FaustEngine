#include "Scene.h"
#include "Component.h"

namespace game
{
	void Scene::update()
	{
		for (uint i = 0; i < ComponentsUpdateOrderSize; i++)
		{
			_updateList[i].clear();
		}
		geometry::Frustum frustum(_camera.projection.Value);
		_tree->foreachLeaf(&frustum, this, &Scene::addLeaf);

		UpdateArgs e;
		e.scene = this;
		for (uint i = 0; i < ComponentsUpdateOrderSize; i++)
		{
			for (uint j = 0; j < _updateList[i].size(); j++)
			{
				_updateList[i][j]->update(e);
			}
		}
	}

	void Scene::invalidate(Entity* entity)
	{
		_tree->place(entity->treeLeaf);
	}

	void Scene::addLeaf(geometry::QuadTreeLeaf* leaf)
	{
		Entity* entity = static_cast<Entity*>(leaf->userData);
		for (uint i = 0; i < entity->components.size(); i++)
		{
			Component* component = entity->components[i];
			_updateList[component->updateOrder].push_back(component);
		}
	}
}