#ifndef GAME_COMPONENTS_FACTORY_H
#define GAME_COMPONENTS_FACTORY_H

#include "game_classes.h"
#include <BaseObjects.h>

namespace game
{
	class IComponentConstructor: IBaseObject
	{
	public:
		virtual Component* createComponent() = 0;
	};

	template <typename T>
	class ComponentConstructor : public IComponentConstructor
	{
	public:
		typedef Component*(T::*constructor)();
		ComponentConstructor(T* instance, constructor* constructor);
		virtual Component* createComponent() OVERRIDE;
	private:
		constructor _constructor;
		T* _instance;
	};

	class ComponentsFactory
	{
	public:
		Component* createComponent(const char* name);		
		void addConstructor(const char* name, IComponentConstructor* constructor);
		template <typename T>
		void addConstructor(const char* name, T* delegateInstance, typename ComponentConstructor<T>::constructor delegateMethod);
		typedef std::map<std::string, IComponentConstructor*> ConstructorsMap;
	private:
		ConstructorsMap _constructors;
	};

	template <typename T>
	ComponentConstructor<T>::ComponentConstructor(T* instance, constructor* constructor)
	{
		_instance = instance;
		_constructor = constructor;
	}

	template <typename T>
	Component* ComponentConstructor<T>::createComponent()
	{
		return (_instance->*_constructor)();

	}

	template <typename T>
	void ComponentsFactory::addConstructor(const char* name, T* delegateInstance, typename ComponentConstructor<T>::constructor delegateMethod)
	{
		addConstructor(name, new ComponentConstructor<T>(delegateInstance, delegateMethod));
	}
}

#endif /*GAME_COMPONENTS_FACTORY_H*/