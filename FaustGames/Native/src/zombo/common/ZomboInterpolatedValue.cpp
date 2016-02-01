#include "ZomboInterpolatedValue.h"
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
	/*
	ZomboInterpolatedValue::ZomboInterpolatedValue(): _ease(&ZomboEase::SmoothQuadric), _value(0), _source(0), _target(0), _duration(0.3f), _time(0.0f), _minVelocity(1.0f), _actualDuration(0.0f)
	{
	}

	ZomboInterpolatedValue::ZomboInterpolatedValue(float value) : _ease(&ZomboEase::SmoothQuadric), _value(value), _source(value), _target(value), _duration(0.3f), _time(0.0f), _minVelocity(1.0f), _actualDuration(0.0f)
	{
	}

	void ZomboInterpolatedValue::update()
	{
		if (_actualDuration < core::Math::Epsilon)
		{
			_value = _target;
			return;
		}
		_time += ZomboGameEnvironment::ellapsedSeconds;
		if (_time >= _actualDuration)
		{
			_value = _target;
			_actualDuration = 0.0f;
			return;
		}
		float nt = _ease->ease(_time / _actualDuration);
		_value = core::Math::lerp(_source, _target, nt);
	}

	float ZomboInterpolatedValue::getValue() const
	{
		return _value;
	}

	float ZomboInterpolatedValue::getTargetValue() const
	{
		return _target;
	}

	void ZomboInterpolatedValue::setAllValues(float value)
	{
		_value = _target = value;
	}

	void ZomboInterpolatedValue::setValue(float value)
	{
		_value = value;
	}

	void ZomboInterpolatedValue::setTargetValueIfNotEqual(float target)
	{
		if (core::Math::equals(target, _target)) return;
		setTargetValue(target);
	}
	
	void ZomboInterpolatedValue::setTargetValue(float target)
	{
		_source = _value;
		_target = target;
		float v = core::Math::abs(_target - _value);
		float velocity = v / _duration;
		if (velocity < _minVelocity)
		{
			_actualDuration = v / _minVelocity;
		}
		else
		{
			_actualDuration = _duration;
		}
		_time = 0;
	}

	void ZomboInterpolatedValue::setDuration(float duration)
	{
		_duration = duration;
	}

	void ZomboInterpolatedValue::setMinVelocity(float minVelocity)
	{
		_minVelocity = minVelocity;
	}

	void ZomboInterpolatedValue::setEase(IZomboEase* ease)
	{
		_ease = ease;
	}

	bool ZomboInterpolatedValue::isUpdating()
	{
		return _actualDuration >= core::Math::Epsilon;
	}
	*/
	SConfig::SConfig(): _ease(&ZomboEase::SmoothQuadric), _duration(0.3f), _minVelocity(1.0f)
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
