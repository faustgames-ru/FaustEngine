#ifndef ZOMBO_INTERPOLATED_VALUE_H
#define ZOMBO_INTERPOLATED_VALUE_H

#include "../zombo_classes.h"
#include "ZomboGameEnvironment.h"

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
	/*
	class ZomboInterpolatedValue
	{
	public:
		ZomboInterpolatedValue();
		explicit ZomboInterpolatedValue(float value);
		void update();
		float getValue() const;
		float getTargetValue() const;
		void setAllValues(float value);
		void setValue(float value);
		void setTargetValueIfNotEqual(float target);
		void setTargetValue(float target);
		void setDuration(float duration);
		void setMinVelocity(float minVelocity);
		void setEase(IZomboEase* ease);
		bool isUpdating();
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
	*/
	class SConfig
	{
	public:
		static SConfig VerySlow;
		static SConfig Slow;
		static SConfig Default;
		static SConfig Fast;
		SConfig();
		SConfig(float duration, float minVelocity);
		float calcDuration(float len) const;
		float calcU(float len, ulong startTime) const;
		float intepolate(float from, float to, ulong startTime) const;
		core::Vector2 intepolate(const core::Vector2 &from, const core::Vector2 &to, ulong startTime) const;
		core::Vector3 intepolate(const core::Vector3 &from, const core::Vector3 &to, ulong startTime) const;
		static bool equal(float a, float b) ;
		static bool equal(const core::Vector2 &a, const core::Vector2 &b) ;
		static bool equal(const core::Vector3 &a, const core::Vector3 &b);
	private:
		IZomboEase* _ease;
		float _duration;
		float _minVelocity;
	};

	template<typename T>
	class SValue
	{
	public:
		SValue();
		SValue(T value);
		void update();
		T get() const;
		T getTarget() const;
		void setAll(T value);
		void set(T value);
		void setTarget(T target);
		void setConfig(SConfig* config);
		bool isUpdating() const;
	private:
		SConfig *_config;
		T _value;
		T _source;
		T _target;
		ulong _startTime;
	};


	template<typename T>
	SValue<T>::SValue() : _config(&SConfig::Default), _value(0), _source(0), _target(0), _startTime(0)
	{
	}

	template<typename T>
	SValue<T>::SValue(T value) : _config(&SConfig::Default), _value(value), _source(value), _target(value), _startTime(0)
	{
	}

	template<typename T>
	void SValue<T>::update()
	{
		_value = _config->intepolate(_source, _target, _startTime);
	}

	template<typename T>
	T SValue<T>::get() const
	{
		return _value;
	}

	template<typename T>
	T SValue<T>::getTarget() const
	{
		return _target;
	}

	template<typename T>
	void SValue<T>::setAll(T value)
	{
		_source = _target = _value = value;
	}

	template<typename T>
	void SValue<T>::set(T value)
	{
		_value = value;
	}

	template<typename T>
	void SValue<T>::setTarget(T target)
	{
		if (SConfig::equal(_target, target)) return;
		_startTime = ZomboGameEnvironment::gameTime;
		_source = _value;
		_target = target;
	}

	template<typename T>
	void SValue<T>::setConfig(SConfig* config)
	{
		_config = config;
	}

	template<typename T>
	bool SValue<T>::isUpdating() const
	{
		return SConfig::equal(_value, _target);
	}

	typedef SValue<float> SFloat;
	typedef SValue<core::Vector2> SVector2;
	typedef SValue<core::Vector3> SVector3;
}

#endif

