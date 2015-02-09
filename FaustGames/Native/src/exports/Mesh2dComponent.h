#ifndef LLGE_MESH_2D_COMPONENT_H
#define LLGE_MESH_2D_COMPONENT_H


#include "llge.h"
#include "..\entities\entity.h"

namespace llge
{
	class Mesh2dComponent : public IMesh2dComponent
	{
	public:
		virtual void API_CALL updateData(void * mesh2dVertices, void * ushortIndices);
	};
}

#endif /*LLGE_MESH_2D_COMPONENT_H*/