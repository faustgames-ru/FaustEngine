#ifndef OBJECTS_POOL_H
#define OBJECTS_POOL_H

#include "stdafx.h"
#include "core.h"
#include "Lazy.h"

namespace core
{
	template<class T> 
	class ObjectsPool
	{
	private:
		T *_root;
		T **_hollows;
		int _count;
		int _limit;
		int _hollowsCount;
		bool _initialized;
		static ObjectsPool _instance;
		ObjectsPool() 
		{
			int size = T::poolSize();
			_limit = size;
			_hollowsCount = 0;
			_count = 0;
			_hollows = new T*[size];
			_root = (T *)new char[size*sizeof(T)];
		}
		~ObjectsPool() 
		{
			delete [] (char *)_root;
			_root = 0;
			delete [] _hollows;
			_hollows = 0;
			if (_hollowsCount != _count)
			{
				throw std::exception("memory leak detected");
			}

		}
	public:
		static ObjectsPool *instance(){ return &_instance; }
		T * create() 
		{
			if (_hollowsCount > 0)
			{
				T *place = _hollows[_hollowsCount];
				T *instance = new (place)T();
				--_hollowsCount;
				_count++;
				return instance;
			}
			else
			{
				if (_count >= _limit)
					throw std::exception("pool overflow");
				T *place = _root + _count;
				T *instance = new (place)T();
				_count++;
				return instance;
			}
		}
		void release(T * value)
		{
			_hollows[_hollowsCount] = value;
			++_hollowsCount;
			value->~T();
		}
	};

	template <class T>
	ObjectsPool<T> ObjectsPool<T>::_instance;

	class Allocator
	{
	public:
		template<class T>
		static T * create(){ return ObjectsPool<T>::instance()->create(); }
		template<class T>
		static void release(T *value){ ObjectsPool<T>::instance()->release(value); }
	};
}

#endif /*OBJECTS_POOL_H*/