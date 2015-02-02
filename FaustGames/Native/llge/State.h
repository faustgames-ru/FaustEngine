#ifndef STATE_H
#define STATE_H

#include "graphics.h"

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
		void setState(T value)
		{
			_equal = _value == value;
			_value = value;
		}
	protected:
	private:
		T _value;
		bool _equal;
	};

}

#endif /*STATE_H*/