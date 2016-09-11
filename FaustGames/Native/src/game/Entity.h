#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "game_classes.h"
#include "Component.h"
#include "ComponentsFactory.h"
#include "../geometry/Quadtree.h"

namespace game
{
	class Entity
	{
	public:
		Entity();
		std::vector<Component* > components;
		core::Vector3 position;
		
		template<typename TCopmponent>
		TCopmponent* find();
		void addComponent(Component* component);
	private:
	};

	template <typename TCopmponent>
	TCopmponent* Entity::find()
	{
		for (uint i = 0; i < components.size(); i++)
		{
			if(components[i]->getTypeId() == ComponentConstructor<TCopmponent>::typeId)
			{
				return static_cast<TCopmponent* >(components[i]->getInstance());
			}
		}
		return nullptr;
	}
}

#endif /*GAME_SCENE_H*/