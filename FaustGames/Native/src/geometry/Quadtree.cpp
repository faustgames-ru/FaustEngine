#include "Quadtree.h"

namespace geometry
{
	QuadTreeLeaf::QuadTreeLeaf(): userData(nullptr), _node(nullptr), _nodeIndex(-1)
	{
	}

	QuadTreeLeaf::~QuadTreeLeaf()
	{
		remove();
	}

	QuadTreeLeaf* QuadTreeLeaf::create()
	{
		return new QuadTreeLeaf();
	}

	void QuadTreeLeaf::dispose()
	{
		delete this;
	}

	void QuadTreeLeaf::remove()
	{
		if (_node == nullptr) return;
		_node->removeLeaf(_nodeIndex);
		_nodeIndex = -1;
		_node = nullptr;
	}

	QuadTreeNodePool::QuadTreeNodePool(int count)
	{
		_data = new QuadTreeNode[count];
		_position = 0;
	}

	QuadTreeNodePool::~QuadTreeNodePool()
	{
		delete[] _data;
	}

	QuadTreeNode* QuadTreeNodePool::getNodes(int count)
	{
		QuadTreeNode* res = _data + _position;
		_position += count;
		return res;
	}
	
	QuadTreeNode::QuadTreeNode(): _totalLeafs(0), _parent(nullptr), _childs(nullptr), _leafs(nullptr)
	{
	}


	QuadTreeNode::~QuadTreeNode()
	{
		if (_childs != nullptr)
		{
			delete [] _childs;
		}
		if (_leafs !=  nullptr)
		{			
			for (uint i = 0; i < _leafs->size(); i++)
			{
				(*_leafs)[i]->_nodeIndex = -1;
				(*_leafs)[i]->_node = nullptr;
			}			
		}
		delete _leafs;
	}

	void QuadTreeNode::place(QuadTreeLeaf* leaf)
	{
		_totalLeafs++;
		if (_childs == nullptr)
		{
			insertLeaf(leaf);
		}
		if (_childs[0]._aabb.contains(leaf->aabb))
		{
			_childs[0].place(leaf);
		}
		else if (_childs[1]._aabb.contains(leaf->aabb))
		{
			_childs[1].place(leaf);
		}
		else
		{
			insertLeaf(leaf);
		}
	}

	Aabb QuadTreeNode::getAabb() const
	{
		return _aabb;
	}

	void QuadTreeNode::construct(QuadTreeNodePool* pool, const Aabb& aabb, int depth)
	{
		_aabb = aabb;
		if (depth != 0)
		{
			Aabb _left;
			Aabb _right;
			aabb.divide(_left, _right);
			_childs = pool->getNodes(2);
			_childs[0].construct(pool, _left, depth - 1);
			_childs[0]._parent = this;
			_childs[1].construct(pool, _right, depth - 1);
			_childs[1]._parent = this;
		}
		else
		{
			_childs = nullptr;
		}
	}

	void QuadTreeNode::insertLeaf(QuadTreeLeaf* leaf)
	{
		if (_leafs == nullptr)
		{
			_leafs = new std::vector<QuadTreeLeaf* >();
		}
		
		leaf->remove();
		leaf->_nodeIndex = _leafs->size() - 1;
		_leafs->push_back(leaf);		
	}

	void QuadTreeNode::removeLeaf(int index)
	{		
		(*_leafs)[index] = _leafs->back();
		(*_leafs)[index]->_nodeIndex = index;
		_leafs->pop_back();
		QuadTreeNode* node = this;
		while (node != nullptr)
		{
			node->_totalLeafs--;
			node = node->_parent;
		}
	}

	QuadTree::QuadTree(const Aabb& aabb, int depth) : _pool((1 << (depth + 1)) - 1)
	{
		_root = _pool.getNodes(1);
		_root->construct(&_pool, aabb, depth);
	}

	QuadTree::~QuadTree()
	{
	}

	void QuadTree::place(QuadTreeLeaf* leaf)
	{
		_root->place(leaf);
	}
}