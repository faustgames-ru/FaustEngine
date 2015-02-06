#ifndef FIXED_POOL_H
#define FIXED_POOL_H

#include "core_classes.h"
#include "Lazy.h"

namespace core
{	
	template<class T, int size>
	class FixedPool
	{
	private:
		static T _root[size];
		static T *_hollows[size];
		static int _count;
		static int _limit;
		static int _hollowsCount;
	public:
		static inline int getIndex(T *value)
		{
			return value - _root;
		}
		static inline T * create()
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
#if _DEBUG
				if (_count >= _limit)
				{
					//throwException("pool overflow");
				}
#endif		
				T *place = _root + _count;
				T *instance = new (place)T();
				_count++;
				return instance;
			}
		}
		static inline T * getByIndex(int id)
		{
			return _root + id;
		}
		static inline void release(T * value)
		{
			_hollows[_hollowsCount] = value;
			++_hollowsCount;
			value->~T();
		}
	};

	template <class T, int size>
	T FixedPool<T, size>::_root[size];
	
	template <class T, int size>
	T * FixedPool<T, size>::_hollows[size];

	template <class T, int size>
	int FixedPool<T, size>::_count(0);

	template <class T, int size>
	int FixedPool<T, size>::_limit(size);

	template <class T, int size>
	int FixedPool<T, size>::_hollowsCount(0);
}

#endif /*FIXED_POOL_H*/