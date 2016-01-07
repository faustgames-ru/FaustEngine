#include "ZomboBehaviors.h"

namespace zombo
{
	ZomboComponentTypes::e ZomboBehaviors::ComponentType(ZomboComponentTypes::Behaviors);

	ZomboBehaviors* ZomboBehaviors::create()
	{
		return new ZomboBehaviors();
	}

	ZomboBehaviors::ZomboBehaviors()
	{
	}

	ZomboBehaviors::~ZomboBehaviors()
	{
		for (uint i = 0; i < _behaviors.size(); i++)
		{
			delete _behaviors[i];
		}
	}

	void ZomboBehaviors::dispose() const
	{
		delete this;
	}

	void ZomboBehaviors::load(ZomboEntity* entity, ZomboContentBlock* content)
	{
		for (uint i = 0; i < _behaviors.size(); i++)
		{
			//_behaviors[i]->load(entity, content);
		}
	}

	void ZomboBehaviors::update(ZomboEntity* entity, float ellapsedTime)
	{
		for (uint i = 0; i < _behaviors.size(); i++)
		{
			//_behaviors[i]->update(entity, ellapsedTime);
		}
	}
}
