#include "ZomboEntity.h"

namespace zombo
{
	ZomboEntity::ZomboEntity()
	{
		for (uint i = 0; i < ZomboComponentTypes::Count; i++)
		{
			_components[i] = nullptr;
		}
	}

	ZomboEntity::~ZomboEntity()
	{
	}
}
