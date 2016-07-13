#include "ContentValue.h"
#include "ContentObject.h"
#include "ContentArray.h"

namespace content
{
	ContentValue* ContentValue::create()
	{
		return new ContentValue();
	}

	void ContentValue::dispose() const
	{
		delete this;
	}

	ContentValue::ContentValue():
		_type(ContentValueType::None),
		_intValue(0), 
		_floatValue(0), 
		_boolValue(false), 
		_stringValue(""), 
		_object(nullptr), 
		_array(nullptr)
	{
	}

	ContentValue::~ContentValue()
	{
		if (_object != nullptr)
		{
			delete _object;
		}
		if (_array != nullptr)
		{
			delete _array;
		}
	}

	ContentValueType::e ContentValue::getType() const
	{
		return _type;
	}

	int ContentValue::asInt() const
	{
		return _intValue;
	}

	float ContentValue::asFloat() const
	{
		return _floatValue;
	}

	bool ContentValue::asBool() const
	{
		return _boolValue;
	}

	const char* ContentValue::asString() const
	{
		return _stringValue.c_str();
	}

	std::string ContentValue::toString() const
	{
		switch (_type)
		{
		case ContentValueType::None: return "null";
		case ContentValueType::Int: return core::Convert::toString(_intValue);
		case ContentValueType::Float: return core::Convert::toString(_floatValue);
		case ContentValueType::Bool: return core::Convert::toString(_boolValue);
		case ContentValueType::String: return _stringValue;
		case ContentValueType::Array:
			if (_array == nullptr)
				return "null";
			return _array->toString();
		case ContentValueType::Object:
			if (_object == nullptr)
				return "null";
			return _object->toString();
		default: 
			return "null";
		}
	}

	ContentObject* ContentValue::asObject()
	{
		_type = ContentValueType::Object;
		if (_object == nullptr)
		{
			_object = new ContentObject();
		}
		return _object;
	}

	ContentArray* ContentValue::asArray()
	{
		_type = ContentValueType::Array;
		if (_array == nullptr)
		{
			_array = new ContentArray();
		}
		return _array;
	}

	void ContentValue::setAsInt(int value)
	{
		_type = ContentValueType::Int;
		_intValue = value;
	}

	void ContentValue::setAsBool(int value)
	{
		_type = ContentValueType::Bool;
		_boolValue = value == 0 ? false : true;
	}

	void ContentValue::setAsFloat(float value)
	{
		_type = ContentValueType::Float;
		_floatValue = value;
	}

	void ContentValue::setAsString(const char* value)
	{
		_type = ContentValueType::String;
		_stringValue = value;
	}

	static std::string _emptyString("");
}
