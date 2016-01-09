#ifndef ZOMBO_INTERPOLATED_VALUE_H
#define ZOMBO_INTERPOLATED_VALUE_H

#include "../zombo_classes.h"

namespace zombo
{
	class IZomboEase : IBaseObject
	{
	public:
		virtual float ease(float t) = 0;
	};

	class ZomboEaseLinear : public IZomboEase
	{
	public:
		virtual float ease(float t) OVERRIDE;
	};

	class ZomboEaseSmoothQuadric : public IZomboEase
	{
	public:
		virtual float ease(float t) OVERRIDE;
	};

	class ZomboEase
	{
	public:
		static ZomboEaseLinear Linear;
		static ZomboEaseSmoothQuadric SmoothQuadric;
	};

	class ZomboInterpolatedValue
	{
	public:
		ZomboInterpolatedValue();
		explicit ZomboInterpolatedValue(float value);
		void update();
		float getValue() const;
		float getTargetValue() const;
		void setValue(float value);
		void setTargetValue(float target);
		void setDuration(float duration);
		void setMinVelocity(float minVelocity);
	private:
		IZomboEase* _ease;
		float _value;
		float _source;
		float _target;
		float _duration;
		float _time;
		float _minVelocity;
		float _actualDuration;
	};
}

#endif

