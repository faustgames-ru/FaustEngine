#ifndef ZOMBO_VALUES_ANIMATOR_H
#define ZOMBO_VALUES_ANIMATOR_H

#include "../zombo_classes.h"
#include "SEntry.h"

namespace zombo
{		
	typedef SEntry<float> SEntryFloat;
	typedef SEntry<float> SEntryVector2;
	typedef SEntry<float> SEntryVector3;

	template<typename T>
	class SAnimator
	{
	public:
		SAnimator();
		void animate(T *value, T target, SConfig* config = &SConfig::Default);
		bool animating(const T *value);
		void terminate(T *value);
		T getTarget(const T* value);
		void update();
	private:
		typedef std::map<const T *, SEntry<T> *> MapType;
		std::vector<const T *> _eraseList;
		MapType _values;
	};

	template <typename T>
	SAnimator<T>::SAnimator()
	{
	}

	template <typename T>
	void SAnimator<T>::animate(T* value, T target, SConfig* config = &SConfig::Default)
	{
		typename MapType::iterator it = _values.find(value);
		SEntry<T>* entry;
		if (it == _values.end())
		{
			_values[value] = entry = new SEntry<T>(value);
		}
		else
		{
			entry = _values[value];
		}
		entry->setConfig(config);
		entry->setTarget(target);
	}

	template <typename T>
	bool SAnimator<T>::animating(const T* value)
	{
		typename MapType::iterator it = _values.find(value);
		return it != _values.end();
	}

	template <typename T>
	void SAnimator<T>::terminate(T* value)
	{
		typename MapType::iterator it = _values.find(value);
		if (it == _values.end()) return;
		it->second->terminate();
		_values.erase(it);
	}

	template <typename T>
	T SAnimator<T>::getTarget(const T* value)
	{
		typename MapType::iterator it = _values.find(value);
		if (it == _values.end()) return *value;
		return it->second->getTarget();
	}

	template <typename T>
	void SAnimator<T>::update()
	{
		for (typename MapType::iterator i = _values.begin(); i != _values.end(); ++i)
		{
			i->second->update();
			if (!i->second->isUpdating())
			{
				_eraseList.push_back(i->first);
			}
		}
		for (uint i = 0; i < _eraseList.size(); i++)
		{
			typename MapType::iterator it = _values.find(_eraseList[i]);
			delete it->second;
			_values.erase(it);
		}
		_eraseList.clear();
	}

	typedef SAnimator<float> AnimatorFloat;
	typedef SAnimator<core::Vector2> AnimatorVector2;


	class Animators
	{
	public:
		static AnimatorFloat Float;
		static AnimatorVector2 Vector2;
		static void update();
	};
}

#endif /* ZOMBO_VALUES_ANIMATOR_H */