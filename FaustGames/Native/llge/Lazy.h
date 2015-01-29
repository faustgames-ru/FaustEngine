#ifndef LAZY_H
#define LAZY_H

#include "stdafx.h"
#include "core.h"

namespace core
{	
	template<typename T>
	class Lazy
	{
	public:
		typedef T *(*instanciator)();
		typedef void(*deinstanciator)(T *);
	private:
		T * _value;
		instanciator _constructor;
		deinstanciator _destructor;
	public:
		
		Lazy(instanciator constructor, deinstanciator destructor)
		{
			_constructor = constructor;
			_destructor = destructor;
		}

		T * value()
		{
			if (!_value)
				_value = _constructor();
			return _value;
		}
		
		Lazy() :_value(0)
		{
		}
		
		~Lazy()
		{
			if (_value)
				_destructor(_value);
			_value = 0;
		};
	};
}

#endif /*LAZY_H*/