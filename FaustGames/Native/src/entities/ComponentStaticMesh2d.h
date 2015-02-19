#ifndef COMPONENT_STATIC_MESH_2D_H
#define COMPONENT_STATIC_MESH_2D_H

#include "entities_classes.h"

namespace entities
{
	struct VertexMesh2d
	{
		float X;
		float Y;
		float Z;
		unsigned short U;
		unsigned short V;
		unsigned int Color;
	};
	
	class ComponentStaticMesh2d
	{
	public:
		int VertexBufferIndex;
		int FirstIndex;
		int IndicesCount;

		static StaticComponentsTypes::e Type;
		static StaticComponentsMasks::e Mask;
		static graphics::StaticBatch<VertexMesh2d> StaticBatch;
	};
}

#endif /*COMPONENT_STATIC_MESH_2D_H*/
