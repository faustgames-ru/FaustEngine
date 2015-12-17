#ifndef CONVERT_H
#define CONVERT_H

#include "core_classes.h"

namespace core
{
	class Convert
	{
	public:
		static float toFloat(int v);
		static float toFloat(const std::string &v);
		static int toInt(float v);
		static int toInt(const std::string &v);
		static std::string toString(bool b);
		static std::string toString(int n);
		static std::string toString(float n);
		static std::string toString(int n, int leadingZeros);
	};
}

#endif /*CONVERT_H*/