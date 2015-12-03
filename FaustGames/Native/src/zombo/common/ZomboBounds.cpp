#include "ZomboBounds.h"

namespace zombo
{
	ZomboComponentTypes::e ZomboBounds::ComponentType(ZomboComponentTypes::Bounds);

	ZomboBounds* ZomboBounds::create()
	{
		return new ZomboBounds();
	}

	ZomboBounds::ZomboBounds() 
		: position(core::Vector3::empty), size(core::Vector2::empty)
	{
	}

	ZomboBounds::~ZomboBounds()
	{
	}

	void ZomboBounds::dispose() const
	{
		delete this;
	}
}
