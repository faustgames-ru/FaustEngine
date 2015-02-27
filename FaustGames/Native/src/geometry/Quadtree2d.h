#ifndef QUAD_TREE_2D_H
#define QUAD_TREE_2D_H

#include "geometry_classes.h"
#include "Aabb2d.h"
#include "Quadtree2dNode.h"

namespace geometry
{	
	class Quadtree2d
	{
	public:
		void clear();
		void clear(const Aabb2d &root);
		int insert(const Aabb2d &aabb, const int userData);
		void remove(int id);
		void filt(const Aabb2d &filter, FilterResult &userDatas) const;
		Quadtree2d();
		~Quadtree2d();
	protected:
	private:
		Quadtree2dNode *getOrCreateNode(const Aabb2d &aabb);
		Quadtree2dNode * _root;
		static Aabb2d DefaultRootSize;
		static const int DepthLimit;
		std::map<int, Quadtree2dItem *, std::less<int>, core::DAlloc> _items;
	};
}

#endif /*QUAD_TREE_2D_H*/
