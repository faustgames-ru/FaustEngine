#include "ComponentsFactory.h"

namespace game
{
	Component* ComponentsFactory::createComponent(const char* name)
	{
		 ConstructorsMap::iterator it = _constructors.find(name);
		 if (it == _constructors.end())
			return nullptr;
		 return it->second->createComponent();
	}

	void ComponentsFactory::addConstructor(const char* name, IComponentConstructor* constructor)
	{
		_constructors[name] = constructor;
	}
}
