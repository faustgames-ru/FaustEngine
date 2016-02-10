#ifndef FUNNEL_H
#define FUNNEL_H

#include "navmesh_classes.h"

namespace navmesh
{
	class Funnel
	{
	public:
		static void makeFunnel(const std::vector<core::Vector2> &portals, std::vector<core::Vector2> &path);
	};
}

#endif /*FUNNEL_H*/
