#include "ZomboContentScene.h"
#include "serialization/ZomboSerializer.h"
#include "serialization/ZomboValue.h"
#include "serialization/ZomboObject.h"
#include "serialization/ZomboArray.h"

namespace zombo
{
	ZomboContentPlatform* ZomboContentPlatform::createFromJson(const char* jsonString)
	{
		ZomboValue* zomboRootValue = ZomboSerializer::deserializeFromJson(jsonString);
		ZomboObject* zomboRoot = zomboRootValue->asObject();

		ZomboContentPlatform* result = new ZomboContentPlatform();
		ZomboArray *leftArray = (*zomboRoot)["left"]->asArray();
		result->left.resize(leftArray->size());
		for (uint i = 0; i < result->left.size(); i++)
		{
			result->left[i] = (*leftArray)[i]->asString();
		}

		ZomboArray *middleArray = (*zomboRoot)["middle"]->asArray();
		result->middle.resize(middleArray->size());
		for (uint i = 0; i < result->middle.size(); i++)
		{
			result->middle[i] = (*middleArray)[i]->asString();
		}		


		ZomboArray *rightArray = (*zomboRoot)["right"]->asArray();
		result->right.resize(rightArray->size());
		for (uint i = 0; i < result->right.size(); i++)
		{
			result->right[i] = (*rightArray)[i]->asString();
		}
		return result;
	}

	void ZomboContentPlatform::dispose() const
	{
		delete this;
	}

	ZomboContentScene* ZomboContentScene::createFromJson(const char* jsonString)
	{
		ZomboValue* zomboRootValue = ZomboSerializer::deserializeFromJson(jsonString);
		ZomboObject* zomboRoot = zomboRootValue->asObject();


		ZomboContentScene* result = new ZomboContentScene();
		result->background = (*zomboRoot)["background"]->asString();

		ZomboArray *resourcesArray = (*zomboRoot)["resources"]->asArray();
		result->resources.resize(resourcesArray->size());
		for (uint i = 0; i < result->resources.size(); i++)
		{
			result->resources[i] = (*resourcesArray)[i]->asString();
		}

		ZomboArray *platformsArray = (*zomboRoot)["platforms"]->asArray();
		result->platforms.resize(platformsArray->size());
		for (uint i = 0; i < result->platforms.size(); i++)
		{
			result->platforms[i] = (*platformsArray)[i]->asString();
		}
		return result;
	}

	void ZomboContentScene::dispose() const
	{
		delete this;
	}
}
