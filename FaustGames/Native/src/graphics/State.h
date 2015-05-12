#ifndef STATE_H
#define STATE_H

#include "graphics_classes.h"

namespace graphics
{
	template<typename T>
	class State
	{
	public:
		State(){}
		State(T value, bool equal)
		{
			_value = value;
			_newValue = value;
			_equal = equal;
		}
		inline T getValue()
		{
			return _value;
		}
		inline bool isEqual()
		{
			return _equal;
		}
		inline void setState(T value)
		{
			_newValue = value;
		}
		inline void applyState()
		{
			_equal = _value == _newValue;
			_value = _newValue;
		}
	protected:
	private:
		T _value;
		T _newValue;
		bool _equal;
	};

}

#endif /*STATE_H*/