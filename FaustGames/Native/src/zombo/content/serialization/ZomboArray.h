#ifndef ZOMBO_ARRAY_H
#define ZOMBO_ARRAY_H

#include "../../zombo_classes.h"
#include "ZomboValue.h"

namespace zombo
{
	class ZomboArray
	{
	public:
		ZomboValue* operator[](int i);
		void resize(int value);
		int size() const;
		std::string toString();
	private:
		std::vector<ZomboValue> _values;
	};
}

#endif /*ZOMBO_ARRAY_H*/