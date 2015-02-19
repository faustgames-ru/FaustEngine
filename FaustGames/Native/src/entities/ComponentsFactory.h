#ifndef COMPONENTS_FACTORY_H
#define COMPONENTS_FACTORY_H

#include "entities_classes.h"
#include "ComponentAabb2d.h"
#include "ComponentStaticMesh2d.h"

namespace entities
{

	class ComponentsFactory
	{
	public:
		static int createStaticComponent(StaticComponentsTypes::e type)
		{
			return -1;;
		}
	};
}

#endif /*COMPONENTS_FACTORY_H*/
