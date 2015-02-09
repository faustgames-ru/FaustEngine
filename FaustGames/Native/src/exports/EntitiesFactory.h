#ifndef LLGE_ENTITIES_FACTORY_H
#define LLGE_ENTITIES_FACTORY_H


#include "llge.h"
#include "Mesh2dEntity.h"
#include "..\entities\entity.h"

namespace llge
{
	class EntitiesFactory : public IEntitiesFactory
	{
		std::vector<Mesh2dEntity *> _meshEntities;
	public:
		virtual IMesh2dEntity * API_CALL createMeshEntity(int verticesCount, int indicesCount);
		virtual void API_CALL disposeMeshEntities();
		virtual void API_CALL dispose();
	};
}

#endif /*LLGE_ENTITIES_FACTORY_H*/