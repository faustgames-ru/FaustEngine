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
			Aabb2d = 0,
			StaticMesh2d = 1,
			ComponentsCount = 2,
		};
	};

	struct StaticComponentsMasks
	{
		enum e
		{
			Aabb2d = 0x01,
			StaticMesh2d = 0x02,
		};
	};

	class EntitiesConstants
	{
	public:
		static const int StaticMeshesReserve = 2048;
		static const int StaticMeshesIndicesReserve = 4096;
		static const int EntitiesReserve = 4096;
		static const int ZLayersHeapSize = 128*1024;
		static const int ZLayersLimit = 4096;
		static const int ZLayersMapSize = 1024;

		static const int TransformNodesLimit = 16384;
		static const int MeshVerticesLimit = 131072;
		static const int MeshIndicesLimit = 131072;
		static const int MeshComponentsLimmit = 16384;
		static const int MeshHeapSize = 4*1024*1024;
	};
}

#endif /*ENTITIES_CLASSES_H*/

