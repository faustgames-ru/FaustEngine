#ifndef STATIC_ARRAY_H
#define STATIC_ARRAY_H

#include "stdafx.h"
#include "core.h"

namespace core
{
	template<class T, int size>
	class StaticArray
	{
	public:
		int limit;
		int count;
		T data[size];
		
		StaticArray() : limit(size), count(0)
		{
		}

		~StaticArray()
		{
		}


		T * addEmpty()
		{
			return &(data[count]);
			++count;
		}
		
		void add(T value)
		{
			data[count] = value;
			++count;
		}
	};
}

#endif /*STATIC_ARRAY_H*/