#include "ZomboValue.h"
#include "ZomboObject.h"
#include "ZomboArray.h"

namespace zombo
{
	ZomboValue::ZomboValue(): 
		_type(ZomboType::None), 
		_intValue(0), 
		_floatValue(0), 
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

	const char* ZomboValue::asString() const
	{
		return _stringValue.c_str();
	}

	ZomboObject* ZomboValue::asObject()
	{
		if (_object == nullptr)
		{
			_object = new ZomboObject();
		}
		return _object;
	}

	ZomboArray* ZomboValue::asArray()
	{
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
