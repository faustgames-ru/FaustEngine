#ifndef QUAD_TREE_2D_ITEM_LIST_H
#define QUAD_TREE_2D_ITEM_LIST_H

#include "geometry_classes.h"
#include "Quadtree2dItem.h"

namespace geometry
{
	class Quadtree2dItemList
	{
	public:
		Quadtree2dItemList() : _first(0)
		{
		}
		~Quadtree2dItemList()
		{
			while (_first)
			{
				Quadtree2dItem *value = _first;
				if (value->Prev)
					value->Prev->Next = value->Next;
				if (value->Next)
					value->Next->Prev = value->Prev;
				if (value == _first)
					_first = value->Next;
				Pool::release(value);
			}
		}

		inline int insert(const Aabb2d &aabb, int nodeID, int userData)
		{
			Quadtree2dItem *value = Pool::create();
			int id = Pool::getIndex(value);
			value->Id = id;
			value->NodeId = nodeID;
			value->Aabb = aabb;
			value->UserData = userData;
			value->Prev = 0;
			value->Next = _first;
			value->List = this;
			_first = value;
			return id;
		}
		inline static void remove(int id)
		{
			Quadtree2dItem *value = Pool::getByIndex(id);
			if (value->Prev)
				value->Prev->Next = value->Next;
			if (value->Next)
				value->Next->Prev = value->Prev;
			Quadtree2dItemList * list = value->List;
			if (value == list->_first)
				list->_first = value->Next;
			Pool::release(value);
		}
		inline Quadtree2dItem * first() const
		{
			return _first;
		}
	protected:
	private:
		Quadtree2dItem * _first;
		typedef core::FixedPool<Quadtree2dItem, GeometryConstants::ObjectsLimit> Pool;
	};
}

#endif /*QUAD_TREE_2D_ITEM_LIST_H*/
