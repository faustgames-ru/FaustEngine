#include "Quadtree2d.h"

namespace geometry
{	
	void Quadtree2d::clear(const Aabb2d &root)
	{
		_root->release();
		_root = Quadtree2dNode::create(root);
	}
	
	int Quadtree2d::insert(const Aabb2d &aabb, const int userData)
	{
		#ifdef __ANDROID__
				__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "insert");
		#endif
		Quadtree2dNode *node = getOrCreateNode(aabb);
		return node->directInsert(aabb, userData);
	}
	
	void Quadtree2d::remove(int id)
	{
		Quadtree2dItemList::remove(id);
	}
	
	void Quadtree2d::filt(const Aabb2d &filter, FilterResult &userDatas) const
	{
		if (!_root) return;
//#ifdef __ANDROID__
//		__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "start query");
//#endif
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