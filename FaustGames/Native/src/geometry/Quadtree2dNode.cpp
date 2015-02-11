#include "Quadtree2dNode.h"

namespace geometry
{
	Quadtree2dNode * Quadtree2dNode::insert(const Aabb2d &aabb, int maxDepth)
	{
		if (!_aabb.contains(aabb))
			return 0;
		if (maxDepth == 0) return this;
		if (_left == 0)
		{
			Aabb2d left;
			Aabb2d right;
			_aabb.divide(left, right);
			_left = Quadtree2dNode::create(left);
			_right = Quadtree2dNode::create(right);
		}
		Quadtree2dNode * result = _left->insert(aabb, maxDepth - 1);
		if (result)
			return result;
		result = _right->insert(aabb, maxDepth - 1);
		if (result)
			return result;
		return this;
	}
	template <typename T>
	std::string to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}
	void Quadtree2dNode::filt(const Aabb2d &filter, FilterResult &result) const
	{
		result.iterations++;
		if (!Aabb2d::cross(_aabb, filter))
			return;
		for (Quadtree2dItem *i = _items.first(); i != 0; i = i->Next)
		{
//#ifdef __ANDROID__
//			__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", to_string(i).c_str());
//#endif
			result.iterations++;
			if (Aabb2d::cross(i->Aabb, filter))
				result.items.add(i->UserData);
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