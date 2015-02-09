// TestTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <new>
#include <exception>
#include <ctime>
#include <vector>

#define SIZE 2
#define DEPTH 20


class GlobalConstants
{
public:
	static const int ObjectsLimit = 8 * 1024 * 1024;
};

namespace core
{	
	template<class T, int size>
	class FixedPool
	{
	private:
		static T *_hollows[size];
		static int _count;
		static int _limit;
		static int _hollowsCount;
		static T * _root;

	public:
		static void initialize()
		{
			_root = (T*)malloc(size*sizeof(T));
		}

		static inline int getCount(){ return _count; }
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
					throw std::exception("pool overflow");
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
	T * FixedPool<T, size>::_root(0);

	template <class T, int size>
	T * FixedPool<T, size>::_hollows[size];

	template <class T, int size>
	int FixedPool<T, size>::_count(0);

	template <class T, int size>
	int FixedPool<T, size>::_limit(size);

	template <class T, int size>
	int FixedPool<T, size>::_hollowsCount(0);

	template<class T>
	class DynamicPool
	{
	private:
		T **_hollows;
		int _count;
		int _limit;
		int _hollowsCount;
		T * _root;

	public:
		DynamicPool(int size)
		{
			_root = (T*)malloc(size*sizeof(T));
			_hollows = (T**)malloc(size*sizeof(T*));
			_count = 0;
			_limit = size;
			_hollowsCount = 0;
		}
		
		void initialize()
		{
			_root = (T*)malloc(size*sizeof(T));
		}

		inline int getCount(){ return _count; }
		inline int getIndex(T *value)
		{
			return value - _root;
		}
		inline T * create()
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
					throw std::exception("pool overflow");
				}
#endif		
				T *place = _root + _count;
				T *instance = new (place)T();
				_count++;
				return instance;
			}
		}
		inline T * getByIndex(int id)
		{
			return _root + id;
		}
		inline void release(T * value)
		{
			_hollows[_hollowsCount] = value;
			++_hollowsCount;
			value->~T();
		}
	};
}

struct ComponentTransform2d
{
	float LocalX;
	float LocalY;
	float LocalZ;
	float LocalPivotX;
	float LocalPivotY;
	float LocalRotation;
	float LocalScale;

	float GlobalX;
	float GlobalY;
	float GlobalZ;
	float GlobalPivotX;
	float GlobalPivotY;
	float GlobalRotation;
	float GlobalScale;

	ComponentTransform2d * Parent;
	ComponentTransform2d * NextNodeId;
	ComponentTransform2d * FirtsChildId;

	typedef core::FixedPool<ComponentTransform2d, GlobalConstants::ObjectsLimit> Pool;

};

class TransformNode
{
public:
	float LocalX;
	float LocalY;
	float LocalZ;
	float LocalPivotX;
	float LocalPivotY;
	float LocalRotation;
	float LocalScale;

	float GlobalX;
	float GlobalY;
	float GlobalZ;
	float GlobalPivotX;
	float GlobalPivotY;
	float GlobalRotation;
	float GlobalScale;

	int FirstChildIndex;
	int ChildsCount;
	
	TransformNode()
	{
		FirstChildIndex = 0;
		ChildsCount = 0;
	}

	typedef core::FixedPool<TransformNode, GlobalConstants::ObjectsLimit> Pool;
};

class Ramdomizer
{
public:
	inline float createRand()
	{
		return (float)(rand() % 200 - 100) / 100.0f;
	}


	Ramdomizer()
	{
		randomIndex = 0;
		for (int i = 0; i < 128; i++)
			randomSwap[i] = createRand();
	}

	float randomSwap[128];
	int randomIndex;

	inline float getRand()
	{
		++randomIndex;
		return randomSwap[randomIndex % 128];
	}

	inline void randomize(TransformNode *node)
	{
		node->LocalX = node->GlobalX = getRand();
		node->LocalY = node->GlobalY = getRand();
		node->LocalZ = node->GlobalZ = getRand();

		node->LocalRotation = node->GlobalRotation = getRand();
		node->LocalScale = node->GlobalScale = getRand();
	}

};

struct Level
{
	int size;
	TransformNode * Data;
	core::DynamicPool<TransformNode> *Pool;
};

class SystemTransform2dLink
{
public:
	Ramdomizer randomizer;
	TransformNode *_root;
	std::vector<Level> _levels;

	void create(TransformNode *root, int count, int level)
	{
		if (level == DEPTH) return;
		for (int i = 0; i < count; i++)
		{
			TransformNode * node = _levels[level + 1].Pool->create();// TransformNode::Pool::create();
			root->FirstChildIndex = _levels[level + 1].Pool->getIndex(node);// TransformNode::Pool::getIndex(node);
			root->ChildsCount = SIZE;
			randomizer.randomize(node);
			for (int j = 1; j < SIZE; j++)
			{
				node = _levels[level + 1].Pool->create();// TransformNode::Pool::create();
				randomizer.randomize(node);
			}
			root++;
		}
	}

	void create()
	{
		_levels.resize(DEPTH+1);
		int size = 4;
		for (int i = 0; i <= DEPTH; i++)
		{
			_levels[i].Pool = new core::DynamicPool<TransformNode>(size);
			size *= SIZE;
		}
		
		_root = _levels[0].Pool->create(); //TransformNode::Pool::create();
		TransformNode * node = _root;
		randomizer.randomize(_root);
		size = 1;

		for (int i = 0; i < DEPTH; i++)
		{
			_levels[i].Data = _root;
			_levels[i].size = size;
			create(_root, size, i);
			size *= SIZE;
			_root = TransformNode::Pool::getByIndex(node->FirstChildIndex);
		}
	}
	
	inline void transform(TransformNode *node, TransformNode *parent)
	{
		node->GlobalX = node->LocalX + parent->GlobalX;
		node->GlobalY = node->LocalY + parent->GlobalY;
		node->GlobalZ = node->LocalZ + parent->GlobalZ;

		node->GlobalRotation = node->LocalRotation + parent->GlobalRotation;
		node->GlobalScale = node->LocalScale * parent->GlobalScale;
	}
	static int iterations;
	void parse()
	{	
		int levelsSize = _levels.size();
		for (int i = 0; i < levelsSize; i++)
		{
			int size = _levels[i].size;
			TransformNode *parent = _levels[i].Data;
			for (int j = 0; j < size; j++)
			{
				//_mm_prefetch((char *)(parent + 10), _MM_HINT_T1);
				int innerSize = parent->ChildsCount;
				TransformNode *child = TransformNode::Pool::getByIndex(parent->FirstChildIndex);
				for (int k = 0; k < innerSize; k++, child++)
				{
					_mm_prefetch((char *)(child + 20), _MM_HINT_NTA);

					child->GlobalX = child->LocalX + parent->GlobalX;
					child->GlobalY = child->LocalY + parent->GlobalY;
					child->GlobalZ = child->LocalZ + parent->GlobalZ;

					child->GlobalRotation = child->LocalRotation + parent->GlobalRotation;
					child->GlobalScale = child->LocalScale * parent->GlobalScale;
					//transform(child, parent);
					//iterations++;
				}
				parent++;
			}
		}
	}
};


class SystemTransform2d
{
public:
	typedef core::FixedPool<ComponentTransform2d, GlobalConstants::ObjectsLimit> Pool;

	SystemTransform2d()
	{
	}

	inline float createRand()
	{
		return (float)(rand() % 200 - 100) / 100.0f;
	}

	ComponentTransform2d * createRoot()
	{
		for (int i = 0; i < 128; i++)
			randomSwap[i] = createRand();
		randomIndex = 0;
		blockIndex = 0;
		ComponentTransform2d *root = Pool::create();
		root->FirtsChildId = 0;
		root->NextNodeId = 0;
		root->Parent = 0;
		return root;
	}

	float randomSwap[128];
	int randomIndex;

	inline float getRand()
	{
		++randomIndex;
		return randomSwap[randomIndex % 128];
	}

	inline void randomize(ComponentTransform2d *node)
	{
		node->LocalX = node->GlobalX = getRand();
		node->LocalY = node->GlobalY = getRand();
		node->LocalZ = node->GlobalZ = getRand();

		node->LocalRotation = node->GlobalRotation = getRand();
		node->LocalScale = node->GlobalScale = getRand();
	}

	void *blocks[16*1024];
	int blockIndex;

	void alloc()
	{
		blocks[blockIndex++] = malloc(rand() % 32);
		if (blockIndex > (16 * 1024))
		{
			blockIndex = 0;
			for (int i = 0; i < (16 * 1024); i++)
				free(blocks[i]);
		}

	}

	void create(ComponentTransform2d *root, int depth)
	{		
		randomize(root);
		if (depth == 0) return;
		
		ComponentTransform2d *child = Pool::create();
		
		//alloc();

		child->Parent = root;
		child->NextNodeId = 0;
		child->FirtsChildId = 0;

		root->FirtsChildId = child;
		create(child, depth - 1);

		for (int i = 1; i < SIZE; i++)
		{
			ComponentTransform2d *nextChild = Pool::create();

			//alloc();

			nextChild->NextNodeId = 0;
			nextChild->FirtsChildId = 0;
			nextChild->Parent = root;
			
			//nextChild->NextNodeId = child;
			//root->FirtsChildId = nextChild;

			child->NextNodeId = nextChild;
			child = nextChild;
			create(nextChild, depth - 1);

		}
		/*
		child = root->FirtsChildId;
		while (child)
		{
			///alloc();
			create(child, depth - 1);
			child = child->NextNodeId;
		}
		*/
		
	}

	void notify(int id)
	{
	}

	void update()
	{
		// recalc global coords in transform tree
		//update aabb
	}

	inline void transform(ComponentTransform2d *node, ComponentTransform2d *parent)
	{
		node->GlobalX = node->LocalX + parent->GlobalX;
		node->GlobalY = node->LocalY + parent->GlobalY;
		node->GlobalZ = node->LocalZ + parent->GlobalZ;

		node->GlobalRotation = node->LocalRotation + parent->GlobalRotation;
		node->GlobalScale = node->LocalScale * parent->GlobalScale;
	}

	void parseLinear()
	{
		int count = SystemTransform2d::Pool::getCount();
		ComponentTransform2d * node = SystemTransform2d::Pool::getByIndex(1);
		int stride = sizeof(ComponentTransform2d) / 4;
		for (int i = 1; i < count; i++, node++)
		{
			_mm_prefetch((char *)(node + 20), _MM_HINT_NTA);
			node->GlobalX = node->LocalX + node->Parent->GlobalX;
			node->GlobalY = node->LocalY + node->Parent->GlobalY;
			node->GlobalZ = node->LocalZ + node->Parent->GlobalZ;

			node->GlobalRotation = node->LocalRotation + node->Parent->GlobalRotation;
			node->GlobalScale = node->LocalScale * node->Parent->GlobalScale;
			//transform(node, node->Parent);
		}
	}
	
	static int iterations;

	void parse(ComponentTransform2d *root)
	{
		//iterations++;
		// do recalc transform;
		ComponentTransform2d * node = root->FirtsChildId;
		while (node)
		{
			//transform(node, root);
			
			node->GlobalX = node->LocalX + root->GlobalX;
			node->GlobalY = node->LocalY + root->GlobalY;
			node->GlobalZ = node->LocalZ + root->GlobalZ;

			node->GlobalRotation = node->LocalRotation + root->GlobalRotation;
			node->GlobalScale = node->LocalScale * root->GlobalScale;
			
			if (node->FirtsChildId >= 0)
				parse(node);
			// do transform;
			if (node->NextNodeId >= 0)
			{
				node = node->NextNodeId;
			}
			else
			{
				node = 0;
			}
		}
	}

private:
	
	int _mask;
};

int SystemTransform2d::iterations(0);
int SystemTransform2dLink::iterations(0);

int _tmain(int argc, _TCHAR* argv[])
{
	ComponentTransform2d::Pool::initialize();
	TransformNode::Pool::initialize();

	long start = clock();
	long finish = clock();
	long dt = finish - start;
	
	SystemTransform2d sys;
	SystemTransform2d::iterations = 0;
	ComponentTransform2d *root = sys.createRoot();
	sys.create(root, DEPTH);
	int count = SystemTransform2d::Pool::getCount();

	
	start = clock();
	for (int i = 0; i < 100;i++)
	{
		sys.parseLinear();
	}
	finish = clock();
	dt = finish - start;

	std::cout << dt;
	std::cout << "\n";
	std::cout << count;
	std::cout << "\n";
	std::cout << "\n";


	start = clock();
	for (int i = 0; i < 100; i++)
	{
		sys.parse(root);
	}

	finish = clock();
	dt = finish - start;


	std::cout << dt;
	std::cout << "\n";
	std::cout << SystemTransform2d::iterations;
	std::cout << "\n";
	std::cout << "\n";

	SystemTransform2dLink::iterations = 0;
	SystemTransform2dLink sysLink;
	sysLink.create();


	start = clock();
	for (int i = 0; i < 100; i++)
	{
		sysLink.parse();
	}

	finish = clock();
	dt = finish - start;

	std::cout << dt;
	std::cout << "\n";
	std::cout << SystemTransform2dLink::iterations;
	std::cout << "\n";


	int i;
	std::cin >> i;
	return 0;
}

