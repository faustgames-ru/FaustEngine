#include "ZomboValue.h"
#include "ZomboObject.h"
#include "ZomboArray.h"

namespace zombo
{
	ZomboValue::ZomboValue(): 
		_type(ZomboType::None), 
		_intValue(0), 
		_floatValue(0), 
		_stringValue(nullptr), 
		_object(nullptr), 
		_array(nullptr)
	{
	}

	ZomboValue::~ZomboValue()
	{
		if (_stringValue != nullptr)
		{
			delete _stringValue;
		}
		if (_object != nullptr)
		{
			delete _object;
		}
		if (_array != nullptr)
		{
			delete _array;
		}
	}
}
