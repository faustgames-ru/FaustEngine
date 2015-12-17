#include "ZomboValue.h"
#include "ZomboObject.h"
#include "ZomboArray.h"

namespace zombo
{
	ZomboValue* ZomboValue::create()
	{
		return new ZomboValue();
	}

	void ZomboValue::dispose() const
	{
		delete this;
	}

	ZomboValue::ZomboValue(): 
		_type(ZomboType::None), 
		_intValue(0), 
		_floatValue(0), 
		_boolValue(false), 
		_stringValue(""), 
		_object(nullptr), 
		_array(nullptr)
	{
	}

	ZomboValue::~ZomboValue()
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

	ZomboType::e ZomboValue::getType() const
	{
		return _type;
	}

	int ZomboValue::asInt() const
	{
		return _intValue;
	}

	float ZomboValue::asFloat() const
	{
		return _floatValue;
	}

	bool ZomboValue::asBool() const
	{
		return _boolValue;
	}

	const char* ZomboValue::asString() const
	{
		return _stringValue.c_str();
	}

	std::string ZomboValue::toString() const
	{
		switch (_type)
		{
		case ZomboType::None: return "null";
		case ZomboType::Int: return core::Convert::toString(_intValue);
		case ZomboType::Float: return core::Convert::toString(_floatValue);
		case ZomboType::Bool: return core::Convert::toString(_boolValue);
		case ZomboType::String: return _stringValue;
		case ZomboType::Array: 
			if (_array == nullptr)
				return "null";
			return _array->toString();
		case ZomboType::Object: 
			if (_object == nullptr)
				return "null";
			return _object->toString();
		default: 
			return "null";
		}
	}

	ZomboObject* ZomboValue::asObject()
	{
		_type = ZomboType::Object;
		if (_object == nullptr)
		{
			_object = new ZomboObject();
		}
		return _object;
	}

	ZomboArray* ZomboValue::asArray()
	{
		_type = ZomboType::Array;
		if (_array == nullptr)
		{
			_array = new ZomboArray();			
		}
		return _array;
	}

	void ZomboValue::setAsInt(int value)
	{
		_type = ZomboType::Int;
		_intValue = value;
	}

	void ZomboValue::setAsBool(int value)
	{
		_type = ZomboType::Bool;
		_boolValue = value;
	}

	void ZomboValue::setAsFloat(float value)
	{
		_type = ZomboType::Float;
		_floatValue = value;
	}

	void ZomboValue::setAsString(const char* value)
	{
		_type = ZomboType::String;
		_stringValue = value;
	}

	static std::string _emptyString("");
}
