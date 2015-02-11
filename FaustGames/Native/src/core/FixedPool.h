#ifndef FIXED_POOL_H
#define FIXED_POOL_H

#include "core_classes.h"
#include "Lazy.h"

namespace core
{

	template<class T, int size>
	class FixedDataBlock
	{
	public:
		T *Data;
		FixedDataBlock()
		{
			Data = (T *)malloc(size * sizeof(T));
		}
		~FixedDataBlock()
		{
			free(Data);
		}
	};
	
	template<class T, int size>
	class FixedPool
	{
	private:
		static FixedDataBlock<T, size> _root;
		static T *_hollows[size];
		static int _count;
		static int _limit;
		static int _hollowsCount;
	public:
		static inline int getIndex(T *value)
		{
			return value - _root.Data;
		}
		
		static inline T * alloc()
		{
			if (_hollowsCount > 0)
			{
				T *place = _hollows[_hollowsCount];
				--_hollowsCount;
				_count++;
				return place;
			}
			else
			{
#if _DEBUG
				if (_count >= _limit)
				{
					//throwException("pool overflow");
				}
#endif		
				T *place = _root.Data + _count;
				_count++;
				return place;
			}
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
				T *place = _root.Data + _count;
				T *instance = new (place)T();
				_count++;
				return instance;
			}
		}
		static inline T * getByIndex(int id)
		{
			return _root.Data + id;
		}
		static inline void release(T * value)
		{
			_hollows[_hollowsCount] = value;
			++_hollowsCount;
			value->~T();
		}
	};

	template <class T, int size>
	FixedDataBlock<T, size> FixedPool<T, size>::_root;
	
	template <class T, int size>
	T * FixedPool<T, size>::_hollows[size];

	template <class T, int size>
	int FixedPool<T, size>::_count(0);

	template <class T, int size>
	int FixedPool<T, size>::_limit(size);

	template <class T, int size>
	int FixedPool<T, size>::_hollowsCount(0);

	template<class T>
	class DataBlock
	{
	public:
		int count;
		T *Data;
	};

	template<typename T, int size>
	class ArraysPool
	{
	private:
		static FixedDataBlock<T, size> _position;
		static int _count;
		static int _limmit;
	public:
		inline static T *alloc(int allocSize)
		{
			if (_limmit < (_count + allocSize))
			{
				return 0;
				//throwException("pool overflow");
			}
			T *place = _position.Data + _count;
			_count += allocSize;

			return place;
		}
		inline static void clear()
		{
			_count = 0;
		}
	};

	template <class T, int size>
	FixedDataBlock<T, size> ArraysPool<T, size>::_position;

	template <class T, int size>
	int ArraysPool<T, size>::_limmit(size);

	template <class T, int size>
	int ArraysPool<T, size>::_count(0);

	template<int size>
	class HeapPool
	{
	private:
		static FixedDataBlock<char, size> _position;
		static int _count;
		static int _limmit;
	public:
		inline static void * getDataByOffset(int offset)
		{
			return _position.Data + offset;
		}
		inline static int getOffset(void *data)
		{
			return (char *)data - _position.Data;
		}
		
		inline static void *alloc(int allocSize)
		{
			if (_limmit < (_count + allocSize))
			{
				return 0;
				//throwException("pool overflow");
			}
			void *place = _position.Data + _count;
			_count += allocSize;

			return place;
		}
		inline static void clear()
		{
			_count = 0;
		}
	};

	template <int size>
	FixedDataBlock<char, size> HeapPool<size>::_position;

	template <int size>
	int HeapPool<size>::_limmit(size);

	template <int size>
	int HeapPool<size>::_count(0);
}

#endif /*FIXED_POOL_H*/