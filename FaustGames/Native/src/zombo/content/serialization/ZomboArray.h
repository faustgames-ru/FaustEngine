#ifndef ZOMBO_ARRAY_H
#define ZOMBO_ARRAY_H

#include "../../zombo_classes.h"
#include "ZomboValue.h"

namespace zombo
{
	class ZomboArray
	{
	public:
		const ZomboValue* operator[](int i) const;
		void resize(int value);
		int size();
	private:
		std::vector<ZomboValue> _values;
	};
}

#endif /*ZOMBO_ARRAY_H*/