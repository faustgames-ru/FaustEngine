#ifndef QUAD_TREE_2D_NODE_H
#define QUAD_TREE_2D_NODE_H

#include "geometry_classes.h"
#include "Aabb2d.h"
#include "Quadtree2dItemList.h"

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
			Pool::release(this);
		}
		inline static Quadtree2dNode *create(const Aabb2d &aabb)
		{
			Quadtree2dNode *result = Pool::create();
			result->_id = Pool::getIndex(result);
			result->_aabb = aabb;
			return result;
		}

		Quadtree2dNode * insert(const Aabb2d &aabb);
		void filt(const Aabb2d &filter, FilterResult &result) const;
		int directInsert(const Aabb2d &aabb, const int userData);

	protected:
	private:
		Aabb2d _aabb;
		Quadtree2dItemList _items;
		int _id;
		Quadtree2dNode *_left;
		Quadtree2dNode *_right;
		typedef core::FixedPool<Quadtree2dNode, GlobalConstants::ObjectsLimit> Pool;
	};
}

#endif /*QUAD_TREE_2D_NODE_H*/
