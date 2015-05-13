#ifndef MATH_H
#define MATH_H

#include "core_classes.h"

namespace core
{
	class Math
	{
	public:
		static const float Epsilon;
		static const float MaxValue;

		static bool equals(float a, float b, float epsilon = Epsilon);
		static float abs(float a);
		static float saturate(float a);
		static float trunc(float a);
	};
}

#endif /*MATH_H*/