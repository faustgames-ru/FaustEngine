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
}
