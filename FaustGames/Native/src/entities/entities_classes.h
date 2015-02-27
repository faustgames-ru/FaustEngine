#ifndef ENTITIES_CLASSES_H
#define ENTITIES_CLASSES_H

#include "entities_platform.h"
#include "entities_references.h"

namespace entities
{
	struct ComponentType
	{
		enum e
		{
			Aadd2d = 0,
			Transform2d = 1,
			Render2d = 2,
			ComponentsCount = 3,
		};
	};

	class EntitiesConstants
	{
	public:
	};

	class Entity;
	class Component;

	typedef std::map<int, Entity*> EntitiesMap;

}

#endif /*ENTITIES_CLASSES_H*/

