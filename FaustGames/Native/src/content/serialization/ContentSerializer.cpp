#include "ContentSerializer.h"
#include "ContentObject.h"
#include "ContentArray.h"

namespace content
{
	cJSON* fillFromZomboValue(ContentValue *value);
	void fillFromCJson(cJSON* json, ContentValue *result);

	void fillJsonArray(ContentArray *arr, cJSON* target)
	{
		for (uint i = 0; i < arr->size(); i++)
		{
			cJSON_AddItemToArray(target, fillFromZomboValue((*arr)[i]));
		}
	}

	void fillZomboArray(cJSON* json, ContentArray *target)
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

	void fillJsonObject(ContentObject *obj, cJSON* target)
	{
		ContentObject::ValuesMap map = obj->getValuesMap();
		for (ContentObject::ValuesMap::iterator i = map.begin(); i != map.end(); ++i)
		{
			cJSON_AddItemToObject(target, i->first.c_str(), fillFromZomboValue(i->second));
		}
	}


	
	void fillZomboObject(cJSON* json, ContentObject *target)
	{
		cJSON *item = json->child;
		while (item != nullptr)
		{
			fillFromCJson(item, (*target)[item->string]);
			item = item->next;
		}
	}

	void fillFromCJson(cJSON* json, ContentValue *result)
	{
		ContentArray *values;
		ContentObject *value;
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

	cJSON* fillFromZomboValue(ContentValue *value)
	{
		cJSON* arr;
		cJSON* obj;
		switch (value->getType())
		{
		case ContentValueType::None:
			break;
		case ContentValueType::Bool:
			return cJSON_CreateBool(value->asBool()?1:0);
		case ContentValueType::Int:
			return cJSON_CreateNumber(value->asInt());
		case ContentValueType::Float:
			return cJSON_CreateNumber(value->asFloat());
		case ContentValueType::String:
			return cJSON_CreateString(value->asString());
		case ContentValueType::Array:
			arr = cJSON_CreateArray();
			fillJsonArray(value->asArray(), arr);
			return arr;
		case ContentValueType::Object:
			obj = cJSON_CreateObject();
			fillJsonObject(value->asObject(), obj);
			return obj;
		default: break;
		}
	}
	
	ContentValue* ContentSerializer::deserializeFromJson(const char* jsonString)
	{
		cJSON* json = cJSON_Parse(jsonString);
		ContentValue *result = ContentValue::create();
		fillFromCJson(json, result);
		cJSON_Delete(json);
		return result;
	}

	std::string ContentSerializer::serializeToJson(ContentValue* value)
	{
		cJSON* json = fillFromZomboValue(value);
		char *data = cJSON_PrintBuffered(json, 128 * 1024, 1);
		std::string result(data);
		free(data);
		cJSON_Delete(json);
		return result;
	}
}
