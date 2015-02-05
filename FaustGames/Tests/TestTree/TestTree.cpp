// TestTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <new>
#include <exception>
#include <ctime>
#include <vector>


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
};

class SystemTransform2dNode
{
public:
	SystemTransform2dNode * Parent;
	ComponentTransform2d *Item;
	std::vector<SystemTransform2dNode *> Childs;

	SystemTransform2dNode()
	{
		Parent = 0;
		Item = new ComponentTransform2d();// Pool::create();

	}
	//typedef core::FixedPool<ComponentTransform2d, GlobalConstants::ObjectsLimit> Pool;
	//typedef core::FixedPool<SystemTransform2dNode, GlobalConstants::ObjectsLimit> ExPool;
};

class SystemTransform2dLink
{
public:
	SystemTransform2dNode *Root;
	inline float createRand()
	{
		return (float)(rand() % 200 - 100) / 100.0f;
	}

	SystemTransform2dNode * createRoot()
	{
		Root = new SystemTransform2dNode();// SystemTransform2dNode::ExPool::create();
		Root->Parent = 0;
		return Root;
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
		node->LocalX = node->GlobalY = getRand();
		node->LocalX = node->GlobalZ = getRand();

		node->LocalRotation = node->GlobalRotation = getRand();
		node->LocalScale = node->GlobalScale = getRand();
	}

	void create(SystemTransform2dNode *root, int depth)
	{
		randomize(root->Item);
		if (depth == 0) return;
		for (int i = 0; i < 16; i++)
		{
			SystemTransform2dNode * child = new  SystemTransform2dNode();// SystemTransform2dNode::ExPool::create();
			root->Childs.push_back(child);
			create(child, depth-1);
		}
	}

	inline void transform(ComponentTransform2d *node, ComponentTransform2d *parent)
	{
		node->GlobalX = node->LocalX + parent->GlobalX;
		node->GlobalY = node->LocalY + parent->GlobalY;
		node->GlobalZ = node->LocalZ + parent->GlobalZ;

		node->GlobalRotation = node->LocalRotation + parent->GlobalRotation;
		node->GlobalScale = node->LocalScale * parent->GlobalScale;
	}

	void parse(SystemTransform2dNode *root)
	{
		int size = root->Childs.size();
		for (int i = 0; i < size; i++)
		{
			transform(root->Childs[i]->Item, root->Item);
			parse(root->Childs[i]);
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
		node->LocalX = node->GlobalY = getRand();
		node->LocalX = node->GlobalZ = getRand();

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

		for (int i = 0; i < 16; i++)
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
		ComponentTransform2d * node;
		for (int i = 1; i < count; i++)
		{
			node = SystemTransform2d::Pool::getByIndex(i);
			//node->GlobalX = node->LocalX + node->Parent->GlobalX;
			//node->GlobalY = node->LocalY + node->Parent->GlobalY;
			//node->GlobalZ = node->LocalZ + node->Parent->GlobalZ;

			//node->GlobalRotation = node->LocalRotation + node->Parent->GlobalRotation;
			//node->GlobalScale = node->LocalScale * node->Parent->GlobalScale;
			transform(node, node->Parent);
		}
	}
	
	static int iterations;

	void parse(ComponentTransform2d *root)
	{
		iterations++;
		// do recalc transform;
		ComponentTransform2d * node = root->FirtsChildId;
		while (node)
		{
			transform(node, root);
			/*
			node->GlobalX = node->LocalX + node->Parent->GlobalX;
			node->GlobalY = node->LocalY + node->Parent->GlobalY;
			node->GlobalZ = node->LocalZ + node->Parent->GlobalZ;

			node->GlobalRotation = node->LocalRotation + node->Parent->GlobalRotation;
			node->GlobalScale = node->LocalScale * node->Parent->GlobalScale;
			*/
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

int _tmain(int argc, _TCHAR* argv[])
{
	//SystemTransform2dNode::Pool::initialize();
	//SystemTransform2dNode::ExPool::initialize();

	long start = clock();
	long finish = clock();
	long dt = finish - start;
	/*
	SystemTransform2d sys;
	SystemTransform2d::iterations = 0;
	ComponentTransform2d *root = sys.createRoot();
	sys.create(root, 5);
	int count = SystemTransform2d::Pool::getCount();

	
	start = clock();
	for (int i = 0; i < 10;i++)
	{
		sys.parseLinear();
	}
	finish = clock();
	dt = finish - start;

	std::cout << dt;
	std::cout << "\n";
	

	start = clock();
	for (int i = 0; i < 10; i++)
	{
		sys.parse(root);
	}

	finish = clock();
	dt = finish - start;


	std::cout << dt;
	std::cout << "\n";
	*/
	SystemTransform2dLink sysLink;
	SystemTransform2dNode *rootLink = sysLink.createRoot();
	sysLink.create(rootLink, 4);


	start = clock();
	for (int i = 0; i < 10; i++)
	{
		sysLink.parse(rootLink);
	}

	finish = clock();
	dt = finish - start;

	std::cout << dt;
	std::cout << "\n";
	

	int i;
	std::cin >> i;
	return 0;
}

