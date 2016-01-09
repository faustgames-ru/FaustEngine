#include "Math.h"

namespace core
{
	const float Math::Epsilon(1e-8f);
	const float Math::MaxValue(1e10f);
	const float Math::Pi(3.14159265359f);

	float Math::abs(float a)
	{
		return a > 0 ? a : -a;
	}

	float Math::saturate(float a)
	{
		if (a < 0.0f)
			return 0.0f;
		if (a > 1.0f)
			return 1.0f;
		return a;
	}

	float Math::trunc(float a)
	{
		return ::trunc(a);
	}

	float Math::round(float a)
	{
		return ::round(a);
	}

	float Math::cos(float a)
	{
		return ::cos(a);
	}

	float Math::sin(float a)
	{
		return ::sin(a);
	}

	float Math::tan(float a)
	{
		return ::tan(a);
	}

	float Math::sqrt(float x)
	{
		return ::sqrt(x);
	}

	float Math::acos(float v)
	{
		return ::acos(v);
	}

	float Math::atan(float v)
	{
		return ::atan(v);
	}

	float Math::atan2(float y, float x)
	{
		return ::atan2(y, x);
	}

	float Math::lerp(float from, float to, float u)
	{
		return from + u * (to - from);
	}

	float Math::asin(float v)
	{
		return ::asin(v);
	}

	bool Math::equals(float a, float b, float epsilon)
	{
		float d = a - b;
		return (d > -epsilon) && (d < epsilon);
	}
}