#include "Quadtree2d.h"

namespace geometry
{	
	void Quadtree2d::clear()
	{
		_root->release();
		_root = 0;
	}
	void Quadtree2d::clear(const Aabb2d &root)
	{
		_root->release();
		_root = Quadtree2dNode::create(root);
	}
	
	int Quadtree2d::insert(const Aabb2d &aabb, const int userData)
	{
		Quadtree2dNode *node = getOrCreateNode(aabb);
		Quadtree2dItem *item = node->directInsert(aabb, userData);
		_items[userData] = item;
		return userData;
	}
	
	void Quadtree2d::remove(int id)
	{
		Quadtree2dItem *item = _items[id];
		_items.erase(id);
		item->Node->remove(item);
	}
	
	void Quadtree2d::filt(const Aabb2d &filter, FilterResult &userDatas) const
	{
		if (!_root) return;
		_root->filt(filter, userDatas);
	}
	
	Quadtree2d::Quadtree2d()
	{
	}
	
	Quadtree2d::~Quadtree2d()
	{
	}
	
	Quadtree2dNode *Quadtree2d::getOrCreateNode(const Aabb2d &aabb)
	{
		if (!_root)
		{
			_root = Quadtree2dNode::create(DefaultRootSize);
		}
		Quadtree2dNode *result = _root->insert(aabb, DepthLimit);
		if (!result)
		{
			return _root;
		}
		return result;
	}
	
	Aabb2d Quadtree2d::DefaultRootSize(
		-(float)GeometryConstants::QuadTreeDefaultSize,
		-(float)GeometryConstants::QuadTreeDefaultSize,
		(float)GeometryConstants::QuadTreeDefaultSize, 
		(float)GeometryConstants::QuadTreeDefaultSize);

	const int Quadtree2d::DepthLimit(GeometryConstants::QuadTreeNodesDepth);
}