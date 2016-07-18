#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "../content/content.h"

namespace game
{
	Scene::Scene(): _tree(nullptr)
	{
	}

	Scene::~Scene()
	{
		// todo: cleanup all
	}

	core::AsyncChain* Scene::load(content::ContentObject* value)
	{
		_tree = new geometry::QuadTree(loadAabb(value), 8);
		loadCamera(value);

		content::ContentArray* entities = (*value)["entities"]->asArray();
		core::AsyncChain* result = core::AsyncChain::create();
		for (uint i = 0; i < entities->size(); i++)
		{
			core::AsyncChain* entityLoadState = loadEntity((*entities)[i]->asObject());
			result->addState(entityLoadState);
		}
		return result;
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

	core::AsyncChain* Scene::loadEntity(content::ContentObject* value)
	{
		Entity* entity = new Entity();
		core::AsyncChain* result = core::AsyncChain::create();
		return result;
	}

	geometry::Aabb Scene::loadAabb(content::ContentObject* value)
	{
		content::ContentArray* aabbValue = (*value)["aabb"]->asArray();
		return geometry::Aabb(
			(*aabbValue)[0]->asFloat(), (*aabbValue)[1]->asFloat(), (*aabbValue)[2]->asFloat(),
			(*aabbValue)[3]->asFloat(), (*aabbValue)[4]->asFloat(), (*aabbValue)[5]->asFloat());
		
	}

	void Scene::loadCamera(content::ContentObject* value)
	{
		content::ContentObject* cameraValue = (*value)["camera"]->asObject();
		content::ContentArray* target = (*cameraValue)["target"]->asArray();
		_camera.target = core::Vector3((*target)[0]->asFloat(), (*target)[1]->asFloat(), (*target)[2]->asFloat());
		_camera.fov = (*cameraValue)["fov"]->asFloat();;
		_camera.scale = (*cameraValue)["scale"]->asFloat();;
		_camera.depth = (*cameraValue)["depth"]->asFloat();;
	}

	void Scene::addLeaf(geometry::QuadTreeLeaf* leaf)
	{
		Component* component = static_cast<Component*>(leaf->userData);
		_updateList[component->updateOrder].push_back(component);
	}
}