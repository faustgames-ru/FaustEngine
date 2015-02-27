#ifndef ALLOCATION_POLICY_H
#define ALLOCATION_POLICY_H

#include <memory>
#include <hash_map>

namespace core
{
	class StandartAllocationPolicy
	{
	public:
		typedef std::size_t size_type;

		static inline void* allocate(size_type cnt)
		{
			return malloc(cnt);
		}

		static inline void deallocate(void* p)
		{
			free(p);
		}
	};

	template<int size>
	class StaticDataBlock
	{
	public:
		char *data;
		int count;
		StaticDataBlock() : count(0)
		{
			data = (char *)malloc(size);
		}
		~StaticDataBlock()
		{
			cleanup();
		}
		void cleanup()
		{
			if (data)
				free(data);
			data = 0;
		}
	};
	
	template<int size>
	class LinearAllocationPolicy
	{
	public:
		static inline void* allocate(std::size_t cnt)
		{
			int i = _block.count;
			_block.count += cnt;
			return _block.data + i;
		}

		static inline void deallocate(void* p)
		{
		}

		static inline void cleanup()
		{
			_block.cleanup();
		}
	private:
		static StaticDataBlock<size> _block;
	};

	template<int size>
	StaticDataBlock<size> LinearAllocationPolicy<size>::_block;

	template<int size>
	class DynamicDataBlock
	{
	public:
		char *data;
		int count;
		int _size;
		DynamicDataBlock() : count(0), data(0), _size(size)
		{
		}
		~DynamicDataBlock()
		{
			if (data)
				free(data);
		}
		inline char * getData()
		{
			if (!data)
				data = (char *)malloc(_size);
			return data;
		}
	};

	template<int BlockSize, int BlocksLimit>
	class LinearBlocksAllocationPolicy
	{
	public:
		static inline void* allocate(std::size_t cnt)
		{
			
			int count = _blocks[_blocksCount].count;
			int newSize = count + cnt;
			if (newSize < BlockSize)
			{
				_blocks[_blocksCount].count = newSize;
				return _blocks[_blocksCount].getData() + count;
			}
			else
			{
				++_blocksCount;
				_blocks[_blocksCount].count = cnt;
				return _blocks[_blocksCount].getData();
			}
		}

		static inline void deallocate(void* p)
		{
		}
	private:
		static int _blocksCount;
		static DynamicDataBlock<BlockSize> _blocks[BlocksLimit];
	};

	template<int BlockSize, int BlocksLimit>
	DynamicDataBlock<BlockSize> LinearBlocksAllocationPolicy<BlockSize, BlocksLimit>::_blocks[BlocksLimit];
	template<int BlockSize, int BlocksLimit>
	int LinearBlocksAllocationPolicy<BlockSize, BlocksLimit>::_blocksCount(0);

	class HolowsAllocationBlock
	{
	public:
		HolowsAllocationBlock *Next;

		HolowsAllocationBlock(int elementSize, int elementsLimit) :
			_data(0), 
			_holes(0), 
			_elementsLimit(elementsLimit),
			_elementSize(elementSize + sizeof(int)), 
			Next(0)
		{	
			_size = _elementsLimit * _elementSize;
		}

		~HolowsAllocationBlock()
		{
			if (_data)
				free(_data);
			if (_holes)
				free(_holes);
		}

		inline char ** getHoles()
		{
			if (!_holes)
				_holes = (char **)malloc(_elementsLimit * sizeof(char *));
			return _holes;
		}

		inline char * getData()
		{
			if (!_data)
				_data = (char *)malloc(_size);
			return _data + sizeof(int);
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

		inline void * allocate()
		{
			if (_holesCount == 0)
			{
				int currentOffset = _count*_elementSize;
				_count ++;
				*((int *)(getData() + currentOffset - sizeof(int))) = _elementSize - sizeof(int);
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
			if ((getData() + (_count * _elementSize)) == p)
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

	class HolowsAllocationBlocks
	{
	public:
		HolowsAllocationBlocks(int elementSize, int elementsLimit) :
			_elementSize(elementSize), 
			_elementsLimit(elementsLimit)
		{
			_first = new HolowsAllocationBlock(_elementSize, _elementsLimit);
		}

		~HolowsAllocationBlocks()
		{
			for (HolowsAllocationBlock *i = _first; i != 0;)
			{
				HolowsAllocationBlock *next = i->Next;
				delete i;
				i = next;
			}
		}

		void * allocate()
		{
			for (HolowsAllocationBlock *i = _first; i != 0; i = i->Next)
			{
				if (i->canAllocate())
					return i->allocate();
			}
			_elementsLimit *= 2;
			HolowsAllocationBlock *newBlock = new HolowsAllocationBlock(_elementSize, _elementsLimit);
			newBlock->Next = _first;
			_first = newBlock;
			return _first->allocate();
		}

		void deallocate(void* p)
		{
			for (HolowsAllocationBlock *i = _first; i != 0; i = i->Next)
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
		HolowsAllocationBlock *_first;
	};

	class SmallBlocksContainer
	{
	public:
		static const int SmallBlocksCount = 16 * 1024;
		SmallBlocksContainer()
		{
			for (int i = 1; i < SmallBlocksCount; i++)
				Blocks[i] = new HolowsAllocationBlocks(i, 2);
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

		HolowsAllocationBlocks * Blocks[SmallBlocksCount];
	};


	class LargeBlocksContainer
	{
	public:
		typedef std::hash_map<std::size_t, HolowsAllocationBlocks *> SizeMap;
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

	class HolowsAllocationPolicy
	{
	public:
		static inline void* allocate(std::size_t cnt)
		{
			
			if (cnt < SmallBlocksContainer::SmallBlocksCount)
			{
				return _smallBlocks.Blocks[cnt]->allocate();
			}
			else			
			{

				LargeBlocksContainer::SizeMap::iterator it = _largeBlocks.Blocks.find(cnt);
				if (it == _largeBlocks.Blocks.end())
					_largeBlocks.Blocks[cnt] = new HolowsAllocationBlocks(cnt, 2);
				return _largeBlocks.Blocks[cnt]->allocate();
			}
		}

		static inline void deallocate(void* p)
		{
			int size = *((int *)p - 1);
			
			if (size < SmallBlocksContainer::SmallBlocksCount)
			{
				_smallBlocks.Blocks[size]->deallocate(p);
			}
			else
			{
				_largeBlocks.Blocks[size]->deallocate(p);
			}
			
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
	
	SmallBlocksContainer HolowsAllocationPolicy::_smallBlocks;
	LargeBlocksContainer HolowsAllocationPolicy::_largeBlocks;
}

#endif /*ALLOCATION_POLICY_H*/
