#ifndef ASTART_H
#define ASTART_H

#include "navmesh_classes.h"

namespace navmesh
{
	class AStarNode;

	typedef std::list<AStarNode *> AStartNodes;


	class AStarNode
	{
	public:
		AStarNode(const core::Vector2 &position);
		void reset();
		core::Vector2 p;
		AStarNode* parent;
		void *userData;
		float g; // distance from start
		float h; // distance to target
		bool closed;
		AStartNodes neigbours;
		static void aStar(AStarNode *start, AStarNode *target, std::vector<AStarNode *> &used);
	};		
}

#endif /*ASTART_H*/
