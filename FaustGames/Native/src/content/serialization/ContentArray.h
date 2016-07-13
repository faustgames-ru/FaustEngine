#ifndef CONTENT_ARRAY_H
#define CONTENT_ARRAY_H

#include "../content_classes.h"
#include "ContentValue.h"

namespace content
{
	class ContentArray
	{
	public:
		ContentValue* operator[](int i);
		void resize(int value);
		int size() const;
		std::string toString();
	private:
		std::vector<ContentValue> _values;
	};
}

#endif /*CONTENT_ARRAY_H*/