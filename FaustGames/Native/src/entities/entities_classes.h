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
			Transform2d = 3,
			Mesh = 4,

			ComponentsCount = 5,
		};
	};

	struct StaticComponentsMasks
	{
		enum e
		{
			Aabb = 0x01,
			Sprite = 0x02,
			LayerIndex = 0x04,
			Transform2d = 0x08,
			Mesh = 0x10,
		};
	};

	class EntitiesConstants
	{
	public:
		static const int EntitiesLimit = 16384;
		static const int TransformNodesLimit = 16384;
		static const int MeshVerticesLimit = 131072;
		static const int MeshIndicesLimit = 131072;
		static const int MeshComponentsLimmit = 16384;
		static const int MeshHeapSize = 4*1024*1024;
	};
}

#endif /*ENTITIES_CLASSES_H*/

