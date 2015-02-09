#include "EntitiesFactory.h"

namespace llge
{
	IMesh2dEntity * API_CALL EntitiesFactory::createMeshEntity(int verticesCount, int indicesCount)
	{
		Mesh2dEntity *result = new Mesh2dEntity(verticesCount, indicesCount);
		_meshEntities.push_back(result);
		return result;
	}

	void API_CALL EntitiesFactory::disposeMeshEntities()
	{
		int size = _meshEntities.size();
		for (int i = 0; i < size; i++)
		{
			delete _meshEntities[i];
		}
	}
	
	void API_CALL EntitiesFactory::dispose()
	{
		disposeMeshEntities();
		delete this;
	}


}
