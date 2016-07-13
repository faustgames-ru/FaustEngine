#ifndef CONTENT_VALUE_H
#define CONTENT_VALUE_H

#include "../content_classes.h"

namespace content
{
	class ContentValue
	{
	public:
		ContentValue();
		~ContentValue();
		ContentValueType::e getType() const;
		int asInt() const;
		float asFloat() const;
		bool asBool() const;
		const char* asString() const;
		std::string toString() const;
		ContentObject* asObject();
		ContentArray* asArray();

		void setAsInt(int value);
		void setAsBool(int value);
		void setAsFloat(float value);
		void setAsString(const char* value);
		static ContentValue* create();
		void dispose() const;
	private:
		ContentValueType::e _type;
		int _intValue;
		float _floatValue;
		bool _boolValue;
		std::string _stringValue;
		ContentObject* _object;
		ContentArray* _array;
	};
}

#endif /*ZOMBO_VALUE_H*/
