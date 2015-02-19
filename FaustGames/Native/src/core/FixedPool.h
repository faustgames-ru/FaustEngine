#ifndef FIXED_POOL_H
#define FIXED_POOL_H

#include "core_classes.h"
#include "Lazy.h"

namespace core
{
	/*
	class FixedBlock
	{
	public:
		char *Data;
		FixedBlock(int size)
		{
			Data = (char *)malloc(size);
		}
		~FixedBlock()
		{
			free(Data);
		}
	};

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
	
	template<typename T>
	class DynamicPool
	{
	private:
		T* _data;
		int _reserve;
		int _count;

		int _hollesCount;
		int *_holles;
	public:
		T * get(int id)
		{
			return _data + id;
		}
		int create()
		{
			int res;
			if (_hollesCount > 0)
			{
				_hollesCount--;
				res = _holles[_hollesCount];
			}
			else
			{
				int newCount = _count + 1;
				if (newCount >= _reserve)
				{
					_reserve *= 2;
					_data = (T*)realloc(_data, _reserve * sizeof(T));
					_holles = (int*)realloc(_holles, _reserve * sizeof(int));
				}
				res = _count;
				_count = newCount;
			}
			new (_data + res)T();
			return res;
		}
		void release(int id)
		{
			_hollows[_hollowsCount] = id;
			++_hollowsCount;
			(_data + id)->~T();
		}

		void releaseAll()
		{
			if (_hollesCount == 0)
			{
				T *p = _data;
				for (int i = 0; i < _count; i++, p++)
				{
					p->~T();
				}
				_count = 0;
			}
			else
			{
				T *p = _data;
				for (int i = 0; i < _count; i++, p++)
				{
					bool hollow = false;
					for (int j = 0; j < _hollesCount; j++)
					{
						if (i == _holles[j])
						{
							hollow = true;
							break;
						}
					}
					if (!hollow)
						p->~T();
				}
				_hollesCount = 0;
				_count = 0;
			}
		}

		DynamicPool(int reserve = 32)
		{
			_reserve = reserve;
			_count = 0;
			_hollesCount = 0;
			_data = (T*)malloc(_reserve * sizeof(T));
			_holles = (int*)malloc(_reserve * sizeof(int));
		}
		~DynamicPool()
		{
			free(_data);
			free(_holles);
		}
	};
	
	class StaticHeap
	{
	private:
		char *_data;
		int _reserve;
		int _size;
	public:
		StaticHeap(int reserve)
		{
			_reserve = reserve;
			_size = 0;
			_data = (char*)malloc(_reserve);
		}
		~StaticHeap()
		{
			free(_data);
		}

		inline void *alloc(int allocSize)
		{
			void * res =  _data + _size;
			_size += allocSize;
			return res;
		}

		inline void clear()
		{
			_size = 0;
		}
	};

	template<typename T>
	class DynamicHeap
	{
	private:
		T *_data;
		int _reserve;
		int _size;
	public:
		DynamicHeap(int reserve)
		{
			_reserve = reserve;
			_size = 0;
			_data = (T*)malloc(_reserve * sizeof(T));
		}
		~DynamicHeap()
		{
			free(_data);
		}
		inline T * get(int index){ return _data + index; }
		inline int alloc(int allocSize)
		{
			int result = _size;
			int newSize = _size + allocSize;
			if (newSize >= _reserve)
			{
				_reserve *= 2;
				while (newSize >= _reserve)
					_reserve *= 2;

				_data = (T*)realloc(_data, _reserve * sizeof(T));
			}
			_size = newSize;
			return result;
		}

		inline void clear()
		{
			_size = 0;
		}
	};

	template<typename T, int startReserve>
	class DynamicHeapPool
	{
	private:
		static DynamicHeap<T> _dataBlock;
	public:
		inline static int alloc(int allocSize)
		{
			return _dataBlock.alloc(allocSize);
		}
		inline static void clear()
		{
			_dataBlock.clear(0);
		}
	};

	template <typename T, int startReserve>
	DynamicHeap<T> DynamicHeapPool<T, startReserve>::_dataBlock(startReserve);

	*/
}

#endif /*FIXED_POOL_H*/