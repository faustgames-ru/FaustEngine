#ifndef ZOMBO_VALUE_H
#define ZOMBO_VALUE_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboValue
	{
	public:
		ZomboValue();
		~ZomboValue();
		ZomboType::e getType() const;
		int asInt() const;
		float asFloat() const;
		const char* asString() const;
		ZomboObject* asObject();
		ZomboArray* asArray();

		void setAsInt(int value);
		void setAsFloat(float value);
		void setAsString(const char* value);
	private:
		ZomboType::e _type;
		int _intValue;
		float _floatValue;
		std::string _stringValue;
		ZomboObject* _object;
		ZomboArray* _array;
	};
}

#endif /*ZOMBO_VALUE_H*/
