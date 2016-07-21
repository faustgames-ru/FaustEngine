#ifndef GAME_COMPONENTS_FACTORY_H
#define GAME_COMPONENTS_FACTORY_H

#include "game_classes.h"
#include <BaseObjects.h>

namespace game
{
	class IComponentConstructor: IBaseObject
	{
	public:
		virtual int getTypeId() = 0;
		virtual Component* createComponent(void*& instance) = 0;
	};

	template <typename TComponent>
	class ComponentConstructor : public IComponentConstructor
	{
	public:
		static int typeId;
		virtual int getTypeId() OVERRIDE;
		virtual Component* createComponent(void*& instance) OVERRIDE;
	};
		
	class ComponentsFactory
	{
	public:
		static ComponentsFactory Default;
		ComponentsFactory();
		Component* createComponent(const char* name);		
		template <typename TComponent>
		void addConstructor(const char* name);
		typedef std::map<std::string, IComponentConstructor*> ConstructorsMap;
	private:
		ConstructorsMap _constructors;
		std::vector<IComponentConstructor*> _constructorsList;
	};

	template <typename TComponent>
	int ComponentConstructor<TComponent>::getTypeId()
	{
		return typeId;
	}

	template <typename TComponent>
	Component* ComponentConstructor<TComponent>::createComponent(void*& instance)
	{
		TComponent* result = new TComponent();
		instance = result;
		return result;
	}

	template <typename TComponent>
	int ComponentConstructor<TComponent>::typeId = -1;

	template <typename TComponent>
	void ComponentsFactory::addConstructor(const char* name)
	{
		ComponentConstructor<TComponent>::typeId = _constructorsList.size();
		ComponentConstructor<TComponent>* constructorInstance = new ComponentConstructor<TComponent>();
		_constructorsList.push_back(constructorInstance);
		_constructors[name] = constructorInstance;
	}
}

#endif /*GAME_COMPONENTS_FACTORY_H*/