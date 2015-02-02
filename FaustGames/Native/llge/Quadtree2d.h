#ifndef OC_TREE_2D_H
#define OC_TREE_2D_H

#include "stdafx.h"
#include "Aabb2d.h"

namespace geometry
{
	class Quadtree2dItem
	{
	public:
	protected:
	private:
		Aabb2d _aabb;
	};

	class Quadtree2dNode
	{
	public:
	protected:
	private:
		Aabb2d _aabb;
		Quadtree2dItem *_items;
		int _count;
	};
	
	class Quadtree2d
	{
	public:
	protected:
	private:
		Quadtree2dNode _allNodes[1024];
		Quadtree2dItem _allItems[1024];
	};
}

#endif /*OC_TREE_2D_H*/
