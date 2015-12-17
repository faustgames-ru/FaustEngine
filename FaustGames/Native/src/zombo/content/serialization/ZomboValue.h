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
		bool asBool() const;
		const char* asString() const;
		std::string toString() const;
		ZomboObject* asObject();
		ZomboArray* asArray();

		void setAsInt(int value);
		void setAsBool(int value);
		void setAsFloat(float value);
		void setAsString(const char* value);
		static ZomboValue* create();
		void dispose() const;
	private:
		ZomboType::e _type;
		int _intValue;
		float _floatValue;
		bool _boolValue;
		std::string _stringValue;
		ZomboObject* _object;
		ZomboArray* _array;
	};
}

#endif /*ZOMBO_VALUE_H*/
