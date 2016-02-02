#include "AStar.h"

namespace navmesh
{
	class AStartNodeComparer
	{
	public:
		bool operator()(AStarNode* left, AStarNode* right) const
		{	// apply operator< to operands
			return (left->g + left->h) > (right->g + right->h);
		}
	};

	AStarNode::AStarNode(const core::Vector2& position): parent(nullptr), userData(nullptr), g(0), h(0), closed(false)
	{
		p = position;
	}

	void AStarNode::reset()
	{
		closed = false;
		parent = nullptr;
		g = 0;
		h = 0;
	}

	void AStarNode::aStar(AStarNode* start, AStarNode* target, std::vector<AStarNode *> &used)
	{
		std::vector<AStarNode *> open;
		target->h = 0;
		start->h = (start->p - target->h).length()*0.01f;
		open.push_back(start);
		used.push_back(start);
		used.push_back(target);
		while (!open.empty())
		{
			std::sort(open.begin(), open.end(), AStartNodeComparer());
			AStarNode *x = open.back();
			open.pop_back();
			if (x->closed)
				continue;
			if (x == target)
			{
				return;
			}
			x->closed = true;
			for (AStartNodes::iterator i = x->neigbours.begin(); i != x->neigbours.end(); ++i)
			{
				AStarNode *n = *i;
				if (n->closed) continue;
				n->parent = x;
				n->g = n->parent->g + (n->p - n->parent->p).length();
				n->h = (target->p - n->p).length()*0.01f;
				open.push_back(n);
				used.push_back(n);
			}
		}
	}
}
