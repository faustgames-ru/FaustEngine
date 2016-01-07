#include "ZomboSerializer.h"
#include "ZomboObject.h"
#include "ZomboArray.h"

namespace zombo
{
	void fillFromCJson(cJSON* json, ZomboValue *result);

	void  fillZomboArray(cJSON* json, ZomboArray *target)
	{
		target->resize(cJSON_GetArraySize(json));
		cJSON *item = json->child;
		int i = 0;
		while (item != nullptr)
		{
			fillFromCJson(item, (*target)[i]);
			++i;
			item = item->next;
		}
	}
	
	void fillZomboObject(cJSON* json, ZomboObject *target)
	{
		cJSON *item = json->child;
		while (item != nullptr)
		{
			fillFromCJson(item, (*target)[item->string]);
			item = item->next;
		}
	}

	void fillFromCJson(cJSON* json, ZomboValue *result)
	{
		ZomboArray *values;
		ZomboObject *value;
		switch (json->type)
		{
		case cJSON_False:
			result->setAsBool(false);
			break;
		case cJSON_True:
			result->setAsBool(true);
			break;
		case cJSON_NULL:
			break;
		case cJSON_Number:
			result->setAsInt(json->valueint);
			result->setAsFloat(static_cast<float>(json->valuedouble));
			break;
		case cJSON_String:
			result->setAsString(json->valuestring);
			break;
		case cJSON_Array:
			values = result->asArray();
			fillZomboArray(json, values);
			break;
		case cJSON_Object:
			value = result->asObject();
			fillZomboObject(json, value);
			break;
		}
	}


	ZomboValue* ZomboSerializer::deserializeFromJson(const char* jsonString)
	{
		cJSON* json = cJSON_Parse(jsonString);
		ZomboValue *result = ZomboValue::create();
		fillFromCJson(json, result);
		cJSON_Delete(json);
		return result;
	}
}
