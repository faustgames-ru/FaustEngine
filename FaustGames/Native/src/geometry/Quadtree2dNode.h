#ifndef QUAD_TREE_2D_NODE_H
#define QUAD_TREE_2D_NODE_H

#include "geometry_classes.h"
#include "Aabb2d.h"
#include "Quadtree2dItem.h"

namespace geometry
{
	class Quadtree2dNode
	{
	public:
		Quadtree2dNode() :_left(0), _right(0) {}
		inline void setAabb(const Aabb2d &aabb){ _aabb = aabb; }
		inline void release()
		{
			if (_left)
				_left->release();
			if (_right)
				_right->release();
			core::Mem::dispose(this);
		}
		inline static Quadtree2dNode *create(const Aabb2d &aabb)
		{
			Quadtree2dNode *result = core::Mem::construct<Quadtree2dNode>();
			result->_aabb = aabb;
			return result;
		}

		Quadtree2dNode * insert(const Aabb2d &aabb, int maxDepth);
		void remove(Quadtree2dItem *item);
		void filt(const Aabb2d &filter, FilterResult &result) const;
		Quadtree2dItem * directInsert(const Aabb2d &aabb, const int userData);
	protected:
	private:
		Aabb2d _aabb;
		Quadtree2dItems _items;
		Quadtree2dNode *_left;
		Quadtree2dNode *_right;
	};
}

#endif /*QUAD_TREE_2D_NODE_H*/
