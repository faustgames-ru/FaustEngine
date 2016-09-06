#include "ZomboContentGame.h"
#include "serialization/ZomboSerializer.h"
#include "serialization/ZomboValue.h"
#include "serialization/ZomboObject.h"

namespace zombo
{

	ZomboContentGame* ZomboContentGame::createFromJson(const char* jsonString)
	{
		ZomboValue* zomboRootValue = ZomboSerializer::deserializeFromJson(jsonString);
		ZomboObject* zomboRoot = zomboRootValue->asObject();
		
		ZomboContentGame* result = new ZomboContentGame();
		result->startup = (*zomboRoot)["startup"]->asString();
		return result;
	}

	void ZomboContentGame::dispose() const
	{
		delete this;
	}
}
