#include "ComponentsFactory.h"
#include "components/FrameAnimation.h"

namespace game
{
	ComponentsFactory ComponentsFactory::Default;

	ComponentsFactory::ComponentsFactory()
	{
		addConstructor<FrameAnimation>("frame_animation");
	}

	Component* ComponentsFactory::createComponent(const char* name)
	{
		 ConstructorsMap::iterator it = _constructors.find(name);
		 if (it == _constructors.end())
			return nullptr;
		 void* instance;
		 Component* result = it->second->createComponent(instance);
		 result->_typeId = it->second->getTypeId();
		 result->_instance = instance;
		 return result;
	}
}
