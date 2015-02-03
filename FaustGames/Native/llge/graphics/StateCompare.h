#ifndef STATE_COMPARE_H
#define STATE_COMPARE_H

#include "graphics_classes.h"

namespace graphics
{
	template<typename T>
	class StateCompare
	{
	public:
		StateCompare(){}
		StateCompare(T value, T prevVaolue, bool equal)
		{
			_value = value;
			_prevValue = prevVaolue;
			_equal = equal;
		}
		inline T getPrevValue()
		{
			return _prevValue;
		}
		inline T getValue()
		{
			return _value;
		}
		inline bool isEqual()
		{
			return _equal;
		}
		void setState(T value)
		{
			_equal = _value == value;
			_prevValue = _value;
			_value = value;
		}
	protected:
	private:
		T _prevValue;
		T _value;
		bool _equal;
	};
}

#endif /*STATE_COMPARE_H*/