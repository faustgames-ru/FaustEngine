#ifndef HOLLOWS_ALLOCATION_POLICY_H
#define HOLLOWS_ALLOCATION_POLICY_H

#include "core_classes.h"
#include "SystemAllocationPolicy.h"

namespace core
{
	class HollowsAllocationBlock
	{
	public:
		static int AllocationBlocksSize;
		HollowsAllocationBlock *Next;

		HollowsAllocationBlock(int elementSize, int elementsLimit) :
			_data(0),
			_holes(0),
			_elementsLimit(elementsLimit),
			_elementSize(elementSize + sizeof(int)),
			Next(0),
			_holesCount(0),
			_count(0)
		{
			_size = _elementsLimit * _elementSize;

		}

		~HollowsAllocationBlock()
		{
			if (_data)
			{
				free(_data);
				AllocationBlocksSize -= _size;
			}
			if (_holes)
			{
				free(_holes);
				AllocationBlocksSize -= _elementsLimit * sizeof(char *);
			}
		}

		inline char ** getHoles()
		{
			if (!_holes)
			{
				_holes = (char **)malloc(_elementsLimit * sizeof(char *));
				AllocationBlocksSize += _elementsLimit * sizeof(char *);
			}
			return _holes;
		}

		inline char * getData()
		{
			if (!_data)
			{
				_data = (char *)malloc(_size);
				AllocationBlocksSize += _size;
			}
			return _data + sizeof(int);
		}

		inline char * getDataOrigin()
		{
			if (!_data)
			{
				_data = (char *)malloc(_size);
				AllocationBlocksSize += _size;
			}
			return _data;
		}

		inline bool contins(void *p)
		{
			if (!_data) return false;
			return (_data <= p) && (p < (_data + _size));
		}
		inline bool canAllocate()
		{
			if (_holesCount == 0)
			{
				return  (_count + 1) <= _elementsLimit;
			}
			return true;
		}

		template <typename T>
		std::string to_string(T value)
		{
			std::ostringstream os;
			os << value;
			return os.str();
		}

		inline void * allocate()
		{
			if (_holesCount == 0)
			{
				int currentOffset = _count*_elementSize;
				_count++;

				*((int *)(getDataOrigin() + currentOffset)) = _elementSize - sizeof(int);

				return getData() + currentOffset;
			}
			else
			{
				--_holesCount;
				return getHoles()[_holesCount];
			}
		}

		inline void deallocate(void *p)
		{
			if ((getData() + ((_count - 1) * _elementSize)) == p)
			{
				--_count;
			}
			else
			{
				getHoles()[_holesCount] = (char *)p;
				_holesCount++;
				if (_holesCount == _count)
				{
					_count = 0;
					_holesCount = 0;
				}
			}
		}
	private:
		char *_data;
		char **_holes;
		int _count;
		int _holesCount;
		int _size;
		int _elementsLimit;
		int _elementSize;
	};

	class HollowsAllocationBlocks
	{
	public:
		HollowsAllocationBlocks(int elementSize, int elementsLimit) :
			_elementSize(elementSize),
			_elementsLimit(elementsLimit)
		{
			_first = new HollowsAllocationBlock(_elementSize, _elementsLimit);
		}

		~HollowsAllocationBlocks()
		{
			for (HollowsAllocationBlock *i = _first; i != 0;)
			{
				HollowsAllocationBlock *next = i->Next;
				delete i;
				i = next;
			}
		}

		void * allocate()
		{
			for (HollowsAllocationBlock *i = _first; i != 0; i = i->Next)
			{
				if (i->canAllocate())
				{
					return i->allocate();
				}
			}
			_elementsLimit *= 2;
			HollowsAllocationBlock *newBlock = new HollowsAllocationBlock(_elementSize, _elementsLimit);
			newBlock->Next = _first;
			_first = newBlock;
			return _first->allocate();
		}

		void deallocate(void* p)
		{
			for (HollowsAllocationBlock *i = _first; i != 0; i = i->Next)
			{
				if (i->contins(p))
				{
					i->deallocate(p);
					break;
				}
			}
		}
	private:
		int _elementSize;
		int _elementsLimit;
		HollowsAllocationBlock *_first;
	};

	class SmallBlocksContainer
	{
	public:
		static const int SmallBlocksCount = 16 * 1024;
		SmallBlocksContainer()
		{
			for (int i = 1; i < SmallBlocksCount; i++)
				Blocks[i] = new HollowsAllocationBlocks(i, 2);
		}
		~SmallBlocksContainer()
		{
			cleanup();
		}
		void cleanup()
		{
			for (int i = 1; i < SmallBlocksCount; i++)
			{
				if (Blocks[i])
					delete Blocks[i];
				Blocks[i] = 0;
			}
		}

		HollowsAllocationBlocks * Blocks[SmallBlocksCount];
	};


	class LargeBlocksContainer
	{
	public:
		typedef std::map<std::size_t, HollowsAllocationBlocks *> SizeMap;
		LargeBlocksContainer()
		{
		}
		~LargeBlocksContainer()
		{
			for (SizeMap::iterator i = Blocks.begin(); i != Blocks.end(); i++)
				delete i->second;
		}
		void cleanup()
		{
			for (SizeMap::iterator i = Blocks.begin(); i != Blocks.end(); i++)
				delete i->second;
			Blocks.clear();
		}
		SizeMap Blocks;
	};

	class HollowsAllocationPolicy
	{
	public:
		static int AllocatedSize;
		static inline void* allocate(std::size_t cnt)
		{
			AllocatedSize += cnt;
			if (cnt < SmallBlocksContainer::SmallBlocksCount)
			{
				return _smallBlocks.Blocks[cnt]->allocate();
			}
			else
			{

				LargeBlocksContainer::SizeMap::iterator it = _largeBlocks.Blocks.find(cnt);
				if (it == _largeBlocks.Blocks.end())
					_largeBlocks.Blocks[cnt] = new HollowsAllocationBlocks(cnt, 2);
				return _largeBlocks.Blocks[cnt]->allocate();
			}
		}
		
		template<typename T>
		static inline T * alloc()
		{
			return (T *)allocate(sizeof(T));
		}

		template<typename T>
		static inline T * construct()
		{
			return new (alloc<T>()) T();
		}

		static inline void deallocate(void* p)
		{
			int size = *((int *)p - 1);
			AllocatedSize -= size;

			if (size < SmallBlocksContainer::SmallBlocksCount)
			{
				_smallBlocks.Blocks[size]->deallocate(p);
			}
			else
			{
				_largeBlocks.Blocks[size]->deallocate(p);
			}

		}

		template<typename T>
		static inline void dispose(T *p)
		{
			p->~T();
			deallocate(p);
		}

		static inline void cleanup()
		{
			_smallBlocks.cleanup();
			_largeBlocks.cleanup();
		}
	private:
		static SmallBlocksContainer _smallBlocks;
		static LargeBlocksContainer _largeBlocks;
	};

	typedef HollowsAllocationPolicy Mem;
	//typedef StandartAllocationPolicy Mem;
}

#endif /*HOLLOWS_ALLOCATION_POLICY_H*/
