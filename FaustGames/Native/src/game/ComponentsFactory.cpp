#include "ComponentsFactory.h"
#include "components/FrameAnimation.h"
#include "components/Mesh.h"

namespace game
{
	ComponentsFactory ComponentsFactory::Default;

	ComponentsFactory::ComponentsFactory()
	{
		addConstructor<FrameAnimation>("frame_animation");
		addConstructor<Mesh>("mesh");
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
