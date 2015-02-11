#ifndef QUAD_TREE_2D_ITEM_H
#define QUAD_TREE_2D_ITEM_H

#include "geometry_classes.h"
#include "Aabb2d.h"

namespace geometry
{
	struct Quadtree2dItem
	{
		int Id;
		int NodeId;
		Aabb2d Aabb;
		int UserData;
		Quadtree2dItem *Prev;
		Quadtree2dItem *Next;
		Quadtree2dItemList *List;

		typedef core::FixedPool<Quadtree2dItem, GeometryConstants::ObjectsLimit> Pool;

	};
}

#endif /*QUAD_TREE_2D_ITEM_H*/
