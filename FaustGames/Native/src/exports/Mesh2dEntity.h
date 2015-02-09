#ifndef LLGE_MESH_2D_ENTITY_H
#define LLGE_MESH_2D_ENTITY_H


#include "llge.h"
#include "..\entities\entity.h"

namespace llge
{
	class Mesh2dEntity : public IMesh2dEntity
	{
		entities::Entity *_instance;
	public:
		Mesh2dEntity(int verticesCount, int indicesCount);
		virtual ITransform2dComponent * API_CALL getTransform();
		virtual IAabb2dComponent * API_CALL getAabbComponent();
		virtual IMesh2dComponent * API_CALL getMesh();
	};
}

#endif /*LLGE_MESH_2D_ENTITY_H*/