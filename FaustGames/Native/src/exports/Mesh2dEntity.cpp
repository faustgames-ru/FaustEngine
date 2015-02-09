#include "Mesh2dEntity.h"

namespace llge
{
	Mesh2dEntity::Mesh2dEntity(int verticesCount, int indicesCount)
	{
		_instance = entities::Entity::Pool::create();
	}

	ITransform2dComponent * API_CALL Mesh2dEntity::getTransform()
	{
		return 0;
	}
	IAabb2dComponent * API_CALL Mesh2dEntity::getAabbComponent()
	{
		return 0;
	}
	IMesh2dComponent * API_CALL Mesh2dEntity::getMesh()
	{
		return 0;
	}

}
