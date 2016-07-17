#include "Scene.h"
#include "Component.h"
#include "Entity.h"

namespace game
{
	Scene::Scene(): _tree(nullptr)
	{
	}

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

	void Scene::invalidate(Component* component)
	{
		_tree->place(component->getAabb(), component->leaf);
	}


	void Scene::addLeaf(geometry::QuadTreeLeaf* leaf)
	{
		Component* component = static_cast<Component*>(leaf->userData);
		_updateList[component->updateOrder].push_back(component);
	}
}