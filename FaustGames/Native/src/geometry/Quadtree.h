#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "geometry_classes.h"
#include "Aabb.h"
#include "Frustum.h"

namespace geometry
{
	class QuadTreeLeaf;
	class QuadTreeNode;
	class QuadTree;


	class QuadTreeLeaf
	{
		friend QuadTreeNode;
	public:
		Aabb aabb;
		void *userData;
		static QuadTreeLeaf* create();
		void dispose();
	private:
		QuadTreeLeaf();
		~QuadTreeLeaf();
		void remove();
		QuadTreeNode* _node;
		int _nodeIndex;
	};
	
	class QuadTreeNodePool
	{
	public:
		QuadTreeNodePool(int count);
		~QuadTreeNodePool();
		QuadTreeNode* getNodes(int count);
	private:
		QuadTreeNode* _data;
		int _position;
	};
	
	class QuadTreeNode
	{
		friend QuadTreeLeaf;
		friend QuadTree;
	public:
		QuadTreeNode();
		~QuadTreeNode();
		void place(const Aabb& aaddb, QuadTreeLeaf* leaf);
		template<typename T>
		void foreachNode(T* delegateInstance, void (T::*delegateMethod)(QuadTreeNode *));
		template<typename T>
		void foreachNode(Frustum* frustum, T* delegateInstance, void (T::*delegateMethod)(QuadTreeNode *));
		template<typename T>
		void foreachLeaf(Frustum* frustum, T* delegateInstance, void (T::*delegateMethod)(QuadTreeLeaf *));
		Aabb getAabb() const;
	private:
		void construct(QuadTreeNodePool* tree, const Aabb &aabb, int depth);
		void insertLeaf(QuadTreeLeaf* leaf);
		void removeLeaf(int index);
		int _totalLeafs;
		QuadTreeNode* _parent;
		Aabb _aabb;
		QuadTreeNode* _childs;
		std::vector<QuadTreeLeaf* > *_leafs;
	};
	
	class QuadTree
	{
	public:
		QuadTree(const Aabb &aabb, int depth);
		~QuadTree();
		void place(const Aabb &aabb, QuadTreeLeaf* leaf);
		
		template<typename T>
		void foreachNode(T* delegateInstance, void (T::*delegateMethod)(QuadTreeNode *));
		template<typename T>
		void foreachNode(Frustum* frustum, T* delegateInstance, void (T::*delegateMethod)(QuadTreeNode *));
		template<typename T>
		void foreachLeaf(Frustum* frustum, T* delegateInstance, void (T::*delegateMethod)(QuadTreeLeaf *));
	private:
		QuadTreeNode* _root;
		QuadTreeNodePool _pool;
	};

	template <typename T>
	void QuadTreeNode::foreachNode(T* delegateInstance, void( T::* delegateMethod)(QuadTreeNode*))
	{
		(delegateInstance->*delegateMethod)(this);
		if (_childs == nullptr) return;
		_childs[0].foreachNode(delegateInstance, delegateMethod);
		_childs[1].foreachNode(delegateInstance, delegateMethod);
	}

	template <typename T>
	void QuadTreeNode::foreachNode(Frustum* frustum, T* delegateInstance, void( T::* delegateMethod)(QuadTreeNode*))
	{
		if (!frustum->include(_aabb)) return;
		(delegateInstance->*delegateMethod)(this);
		if (_childs == nullptr) return;
		_childs[0].foreachNode(frustum, delegateInstance, delegateMethod);
		_childs[1].foreachNode(frustum, delegateInstance, delegateMethod);
	}

	template <typename T>
	void QuadTreeNode::foreachLeaf(Frustum* frustum, T* delegateInstance, void( T::* delegateMethod)(QuadTreeLeaf*))
	{
		if (_totalLeafs == 0) return;
		if (!frustum->include(_aabb)) return;
		if (_leafs != nullptr)
		{
			QuadTreeLeaf* leaf;
			for (uint i = 0; i < _leafs->size(); i++)
			{
				leaf = (*_leafs)[i];
				if (!frustum->include(leaf->aabb)) continue;
				(delegateInstance->*delegateMethod)(leaf);
			}
		}
		if (_childs == nullptr) return;
		_childs[0].foreachLeaf(frustum, delegateInstance, delegateMethod);
		_childs[1].foreachLeaf(frustum, delegateInstance, delegateMethod);
	}

	template <typename T>
	void QuadTree::foreachNode(T* delegateInstance, void( T::* delegateMethod)(QuadTreeNode*))
	{
		_root->foreachNode(delegateInstance, delegateMethod);
	}

	template <typename T>
	void QuadTree::foreachNode(Frustum* frustum, T* delegateInstance, void( T::* delegateMethod)(QuadTreeNode*))
	{
		_root->foreachNode(frustum, delegateInstance, delegateMethod);
	}

	template <typename T>
	void QuadTree::foreachLeaf(Frustum* frustum, T* delegateInstance, void( T::* delegateMethod)(QuadTreeLeaf*))
	{
		_root->foreachLeaf(frustum, delegateInstance, delegateMethod);
	}
}

#endif /*QUAD_TREE_H*/
