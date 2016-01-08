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
		static const float Pi;

		static bool equals(float a, float b, float epsilon = Epsilon);
		static float abs(float a);
		static float saturate(float a);
		static float trunc(float a);
		static float round(float a);
		static float cos(float a);
		static float sin(float a);
		static float tan(float a);
		static float sqrt(float x);
		static float acos(float v);
		static float atan(float v);
		static float atan2(float y, float x);
	};
}

#endif /*MATH_H*/