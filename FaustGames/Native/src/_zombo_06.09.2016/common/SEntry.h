#ifndef ZOMBO_SENTRY_H
#define ZOMBO_SENTRY_H

#include "../zombo_classes.h"
#include "ZomboGameEnvironment.h"
#include "SConfig.h"

namespace zombo
{
	template<typename T>
	class SEntry
	{
	public:
		SEntry();
		SEntry(T *value);
		void update();
		void setTarget(T target);
		void setConfig(SConfig* config);
		bool isUpdating() const;
		void terminate();
		T getTarget();
	private:
		SConfig *_config;
		T* _value;
		T _source;
		T _target;
		ulong _startTime;
	};


	template<typename T>
	SEntry<T>::SEntry() : _config(&SConfig::Default), _value(nullptr), _source(0), _target(0), _startTime(0)
	{
	}

	template<typename T>
	SEntry<T>::SEntry(T *value) : _config(&SConfig::Default), _startTime(0)
	{
		_value = value;
		_source = *value;
		_target = *value;
	}

	template<typename T>
	void SEntry<T>::update()
	{
		*_value = _config->intepolate(_source, _target, _startTime);
	}

	template<typename T>
	void SEntry<T>::setTarget(T target)
	{
		if (SConfig::equal(_target, target)) return;
		_startTime = ZomboGameEnvironment::gameTime;
		_source = *_value;
		_target = target;
	}

	template<typename T>
	void SEntry<T>::setConfig(SConfig* config)
	{
		_config = config;
	}

	template<typename T>
	bool SEntry<T>::isUpdating() const
	{
		return !SConfig::equal(*_value, _target);
	}

	template <typename T>
	void SEntry<T>::terminate()
	{
		*_value = _target;
	}

	template <typename T>
	T SEntry<T>::getTarget()
	{
		return _target;
	}
}

#endif /*ZOMBO_SENTRY_H*/
