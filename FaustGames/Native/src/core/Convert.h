#ifndef CONVERT_H
#define CONVERT_H

#include "core_classes.h"

namespace core
{
	class Convert
	{
	public:
		static std::string toString(int n);
		static std::string toString(int n, int leadingZeros);
	};
}

#endif /*CONVERT_H*/