#ifndef QUAD_TREE_2D_ITEM_H
#define QUAD_TREE_2D_ITEM_H

#include "geometry_classes.h"
#include "Aabb2d.h"

namespace geometry
{
	typedef std::list<Quadtree2dItem *, core::DAlloc> Quadtree2dItems;

	struct Quadtree2dItem
	{
		Quadtree2dNode* Node;
		Quadtree2dItems::const_iterator it;
		Aabb2d Aabb;
		int UserData;
	};
}

#endif /*QUAD_TREE_2D_ITEM_H*/
