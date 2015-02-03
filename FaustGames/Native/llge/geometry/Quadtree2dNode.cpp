#include "Quadtree2dNode.h"

namespace geometry
{
	Quadtree2dNode * Quadtree2dNode::insert(const Aabb2d &aabb)
	{
		if (!_aabb.contains(aabb))
			return 0;
		if (_left == 0)
		{
			Aabb2d left;
			Aabb2d right;
			_aabb.divide(left, right);
			_left = Quadtree2dNode::create(left);
			_right = Quadtree2dNode::create(right);
		}
		Quadtree2dNode * result = _left->insert(aabb);
		if (result)
			return result;
		result = _right->insert(aabb);
		if (result)
			return result;
		return this;
	}

	void Quadtree2dNode::filt(const Aabb2d &filter, FilterResult &result) const
	{
		if (!Aabb2d::cross(_aabb, filter)) return;
		for (Quadtree2dItem *i = _items.first(); i != 0; i = i->Next)
		{
			if (Aabb2d::cross(i->Aabb, filter))
				result.add(i->UserData);
		}
		if (_left)
			_left->filt(filter, result);
		if (_right)
			_right->filt(filter, result);
	}

	int Quadtree2dNode::directInsert(const Aabb2d &aabb, const int userData)
	{
		return _items.insert(aabb, _id, userData);
	}
}