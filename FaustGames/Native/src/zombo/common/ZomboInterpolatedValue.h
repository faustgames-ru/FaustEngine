#ifndef ZOMBO_INTERPOLATED_VALUE_H
#define ZOMBO_INTERPOLATED_VALUE_H

#include "../zombo_classes.h"
#include "ZomboGameEnvironment.h"
#include "SConfig.h"

namespace zombo
{	
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

