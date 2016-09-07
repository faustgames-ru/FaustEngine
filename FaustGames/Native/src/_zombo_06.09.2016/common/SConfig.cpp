#include "SConfig.h"
#include "ZomboGameEnvironment.h"

namespace zombo
{
	ZomboEaseLinear ZomboEase::Linear;
	ZomboEaseSmoothQuadric ZomboEase::SmoothQuadric;

	float ZomboEaseLinear::ease(float t)
	{
		return t;
	}

	float ZomboEaseSmoothQuadric::ease(float t)
	{
		if (t < 0.5f)
		{
			return t*t*2.0f;
		}
		t = 1.0f - t;
		return 1.0f - t*t*2.0f;;
	}


	SConfig::SConfig() : _ease(&ZomboEase::SmoothQuadric), _duration(0.3f), _minVelocity(1.0f)
	{
	}

	SConfig::SConfig(float duration, float minVelocity) : _ease(&ZomboEase::SmoothQuadric), _duration(duration), _minVelocity(minVelocity)
	{
	}

	float SConfig::calcDuration(float len) const
	{
		float velocity = len / _duration;
		if (velocity < _minVelocity)
		{
			return len / _minVelocity;
		}
		return _duration;
	}

	float SConfig::calcU(float len, ulong startTime) const
	{
		float dur = calcDuration(len);
		if (dur < core::Math::Epsilon)
		{
			return 1;
		}
		ulong nanoDuration = ZomboGameEnvironment::getTime(dur);
		ulong time = ZomboGameEnvironment::gameTime;
		ulong ellpased = time - startTime;
		if (ellpased > nanoDuration)
		{
			return 1;
		}
		return _ease->ease(ZomboGameEnvironment::getSeconds(ellpased) / dur);
	}

	float SConfig::intepolate(float from, float to, ulong startTime) const
	{
		float u = calcU(core::Math::abs(to - from), startTime);
		return core::Math::lerp(from, to, u);
	}

	core::Vector2 SConfig::intepolate(const core::Vector2 &from, const core::Vector2 &to, ulong startTime) const
	{
		float u = calcU((to - from).length(), startTime);
		return core::Vector2::lerp(from, to, u);
	}

	core::Vector3 SConfig::intepolate(const core::Vector3 &from, const core::Vector3 &to, ulong startTime) const
	{
		float u = calcU((to - from).length(), startTime);
		return core::Vector3::lerp(from, to, u);
	}

	bool SConfig::equal(float a, float b)
	{
		return core::Math::equals(a, b);
	}

	bool SConfig::equal(const core::Vector2& a, const core::Vector2& b)
	{
		return core::Vector2::equals(a, b);
	}

	bool SConfig::equal(const core::Vector3& a, const core::Vector3& b)
	{
		return core::Vector3::equals(a, b);
	}

	SConfig SConfig::VerySlow(1.2f, 0.25f);
	SConfig SConfig::Slow(0.6f, 0.5f);
	SConfig SConfig::Default;
	SConfig SConfig::Fast(0.15f, 2.0f);

}
