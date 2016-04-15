#ifndef MATH_H
#define MATH_H

#include "core_classes.h"

namespace core
{
	class Math
	{
	public:
		static float Epsilon;
		static float MaxValue;
		static float Pi;

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
		static float lerp(float from, float to, float u);
		static float conic(float p0, float p1, float p2, float u);
		static float cubic(float p0, float p1, float p2, float p3, float u);
		static float asin(float v);
		static float frac(float v);
		static float max(float v0, float v1);
		static float max(float v0, float v1, float v2);
		static float min(float v0, float v1);
		static float exp(float v);
	};
}

#endif /*MATH_H*/