#ifndef ENTITIES_CLASSES_H
#define ENTITIES_CLASSES_H

#include "entities_platform.h"
#include "entities_references.h"

namespace entities
{
	struct StaticComponentsTypes
	{
		enum e
		{
			Aabb = 0,
			Sprite = 1,
			LayerIndex = 2,
			
			ComponentsCount = 3,
		};
	};

	struct StaticComponentsMasks
	{
		enum e
		{
			Aabb = 0x01,
			Sprite = 0x02,
			LayerIndex = 0x04,
		};
	};
}

#endif /*ENTITIES_CLASSES_H*/

