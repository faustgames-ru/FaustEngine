#include "Math.h"

namespace core
{
	const float Math::Epsilon(1e-8f);
	const float Math::MaxValue(1e10f);

	float Math::abs(float a)
	{
		return a > 0 ? a : -a;
	}


	bool Math::equals(float a, float b, float epsilon)
	{
		float d = a - b;
		return (d > -epsilon) && (d < epsilon);
	}
}