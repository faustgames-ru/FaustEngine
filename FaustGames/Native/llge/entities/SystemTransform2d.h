#ifndef SYSTEM_TRANSFORM_2D_H
#define SYSTEM_TRANSFORM_2D_H

#include "entities_classes.h"
#include "ComponentTransform2d.h"

namespace entities
{
	class SystemTransform2d
	{
	public:
		SystemTransform2d() : _mask(StaticComponentsMasks::Transform2d)
		{
		}

		void notify(int id)
		{
		}

		void update()
		{
			// recalc global coords in transform tree
			 //update aabb
		}
	private:
		void parse(ComponentTransform2d *transform)
		{
			// do recalc transform;
			ComponentTransform2d * node = Pool::getByIndex(transform->FirtsChildId);
			while (node)
			{
				// do transform;
				parse(node);
				if (node->NextNodeId > 0)
				{
					node = Pool::getByIndex(node->NextNodeId);
				}
				else
				{
					node = 0;
				}
			}
		}
		int _mask;
		typedef core::FixedPool<ComponentTransform2d, GlobalConstants::ObjectsLimit> Pool;
	};

	class Transform2dNode
	{
	};
	
	class Transform2dTree
	{
	public:
		Transform2dNode *_root;
	};
}

#endif SYSTEM_TRANSFORM_2D_H
