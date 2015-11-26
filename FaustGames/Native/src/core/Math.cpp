#include "Math.h"

namespace core
{
	const float Math::Epsilon(1e-8f);
	const float Math::MaxValue(1e10f);

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

	float Math::cos(float a)
	{
		return ::cos(a);
	}

	float Math::sin(float a)
	{
		return ::sin(a);
	}

	bool Math::equals(float a, float b, float epsilon)
	{
		float d = a - b;
		return (d > -epsilon) && (d < epsilon);
	}
}