#include "ZomboSerializer.h"
#include "ZomboObject.h"
#include "ZomboArray.h"

namespace zombo
{
	cJSON* fillFromZomboValue(ZomboValue *value);
	void fillFromCJson(cJSON* json, ZomboValue *result);

	void  fillJsonArray(ZomboArray *arr, cJSON* target)
	{
		for (uint i = 0; i < arr->size(); i++)
		{
			cJSON_AddItemToArray(target, fillFromZomboValue((*arr)[i]));
		}
	}

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

	void  fillJsonObject(ZomboObject *obj, cJSON* target)
	{
		ZomboObject::ValuesMap map = obj->getValuesMap();
		for (ZomboObject::ValuesMap::iterator i = map.begin(); i != map.end(); ++i)
		{
			cJSON_AddItemToObject(target, i->first.c_str(), fillFromZomboValue(i->second));
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

	cJSON* fillFromZomboValue(ZomboValue *value)
	{
		cJSON* arr;
		cJSON* obj;
		switch (value->getType())
		{
		case ZomboType::None:
			break;
		case ZomboType::Bool:			
			return cJSON_CreateBool(value->asBool()?1:0);
		case ZomboType::Float:
			return cJSON_CreateNumber(value->asFloat());
		case ZomboType::String:
			return cJSON_CreateString(value->asString());
		case ZomboType::Array:
			arr = cJSON_CreateArray();
			fillJsonArray(value->asArray(), arr);
			return arr;
		case ZomboType::Object:
			obj = cJSON_CreateObject();
			fillJsonObject(value->asObject(), obj);
			return obj;
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

	std::string ZomboSerializer::serializeToJson(ZomboValue* value)
	{
		cJSON* json = fillFromZomboValue(value);
		char *data = cJSON_PrintBuffered(json, 128 * 1024, 1);
		std::string result(data);
		free(data);
		cJSON_Delete(json);
		return result;
	}
}
