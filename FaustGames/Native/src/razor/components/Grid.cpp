#include "Grid.h"
#include "../../game/Entity.h"
#include "../../game/services/Drawing.h"

namespace razor
{
	Grid::Grid()
	{
	}

	void Grid::enqueueResources(const game::LoadArgs& e)
	{
		int sizeX = 50;
		int sizeY = 50;
		int step = 5;
		for (int x = -sizeX; x <= sizeX; x+= step)
		{
			for (int y = -sizeY; y <= sizeY; y+= step)
			{
				GridNode* node = new GridNode();
				node->position = core::Vector3(x, y, 0);
				owner->addComponent(node);
			}
		}
	}

	GridNode::GridNode()
	{
		renderOrder = game::RenderOrder::Default;
	}

	void GridNode::enqueueResources(const game::LoadArgs& e)
	{
	}

	void GridNode::loaded()
	{
		halfSize = core::Vector3(0.1f, 0.1f, 0.0f);
	}

	void GridNode::render()
	{
		game::Drawing::Default.pen.color = 0x80808080;
		game::Drawing::Default.fillRect(position, halfSize.toVector2());
	}
}