#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "../content/content.h"
#include "../geometry/Quadtree.h"
#include "ComponentsFactory.h"

namespace game
{
	Scene::Scene(): _tree(nullptr)
	{
	}

	Scene::~Scene()
	{
		// todo: cleanup all
	}

	void Scene::enqueueResources(const LoadArgs& e)
	{
		content::ContentObject* value = e.value->asObject();
		_tree = new geometry::QuadTree(loadAabb(value), 16);
		loadCamera(value);

		content::ContentArray* entities = (*value)["entities"]->asArray();
		for (int i = 0; i < entities->size(); i++)
		{
			LoadArgs args;
			args.content = e.content;
			args.value = (*entities)[i];
			enqueueEntityResources(args);
		}
	}

	void Scene::loaded()
	{
		for (uint i = 0; i < _entities.size(); i++)
		{
			for (uint j = 0; j < _entities[i]->components.size(); j++)
			{
				_entities[i]->components[j]->loaded();
				_tree->place(_entities[i]->components[j]->getAabb(), _entities[i]->components[j]->leaf);
			}
		}
	}


	struct 
	{
		bool operator()(Component* a, Component* b)
		{
			return a->getZOrder() > b->getZOrder();
		}
	} RenderComponentComparer;

	void Scene::update()
	{
		for (uint i = 0; i < ComponentsUpdateOrderSize; i++)
		{
			_updateList[i].clear();
			_renderList[i].clear();
		}
		geometry::Frustum frustum(_camera.transform.Value);
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
		
		_camera.update();
		graphics::UniformValues::projection()->setValue(_camera.transform);
		for (uint i = 0; i < ComponentsUpdateOrderSize; i++)
		{
			std::sort(_renderList[i].begin(), _renderList[i].end(), RenderComponentComparer);
			for (uint j = 0; j < _renderList[i].size(); j++)
			{
				_renderList[i][j]->render();
			}
		}
	}

	void Scene::invalidate(Entity* entity)
	{
		for (uint i = 0; i < entity->components.size(); i++)
		{
			invalidate(entity->components[i]);
		}
	}

	void Scene::invalidate(Component* component)
	{
		_tree->place(component->getAabb(), component->leaf);
	}

	graphics::Camera2d* Scene::camera()
	{
		return &_camera;
	}

	void Scene::enqueueEntityResources(const LoadArgs& e)
	{
		content::ContentObject* value = e.value->asObject();
		Entity* entity = new Entity();
		content::ContentObject::ValuesMap& values = value->getValuesMap();
		for (content::ContentObject::ValuesMap::iterator it = value->getValuesMap().begin(); it != value->getValuesMap().end(); ++it)
		{
			if(it->first == "position")
			{
				/// load entity position;
				content::ContentArray* positionValues = it->second->asArray();
				entity->position.setX((*positionValues)[0]->asFloat());
				entity->position.setY((*positionValues)[1]->asFloat());
				entity->position.setZ((*positionValues)[2]->asFloat());
			}
			else
			{
				Component* component = ComponentsFactory::Default.createComponent(it->first.c_str());
				if (component != nullptr)
				{
					entity->addComponent(component);
					LoadArgs args;
					args.value = it->second;
					args.content = e.content;
					component->enqueueResources(args);
				}
			}
		}
		_entities.push_back(entity);
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
		if (component->updateOrder != UpdateOrder::None)
		{
			_updateList[component->updateOrder].push_back(component);
		}
		if (component->renderOrder != RenderOrder::None)
		{
			_renderList[component->renderOrder].push_back(component);
		}
	}
}