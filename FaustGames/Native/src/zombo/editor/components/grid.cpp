#include "grid.h"
#include "../../../game/Scene.h"
#include "../../../game/services/Drawing.h"
#include "../../../game/Entity.h"

namespace zombo
{
	Grid::Grid()
	{
	}

	void Grid::enqueueResources(const game::LoadArgs& e)
	{
		geometry::Aabb scaneAabb = e.scene->bounds();
		core::Vector3 c = scaneAabb.center();
		core::Vector3 halfSize = scaneAabb.halfSize();
		int sx = static_cast<int>(scaneAabb.Min.getX());
		int fx = static_cast<int>(scaneAabb.Max.getX());
		int dx = 1;

		int sy = static_cast<int>(scaneAabb.Min.getY());
		int fy = static_cast<int>(scaneAabb.Max.getY());
		int dy = 1;

		for (int x = sx; x <= fx; x += dx)
		{
			GridEdge* edge = new GridEdge();
			float z = 0.0f;
			if (x % 10 == 0)
			{
				edge->color = 0x80ffffff;
				z += 0.001f;
			}
			else
			{
				edge->color = 0x20404040;
			}

			edge->position = core::Vector3(x, c.getY(), z);
			edge->halfSize = core::Vector3(core::Math::Epsilon, halfSize.getY(), core::Math::Epsilon);
			edge->p0 = core::Vector3(x, c.getY() - halfSize.getY(), 0.0f);
			edge->p1 = core::Vector3(x, c.getY() + halfSize.getY(), 0.0f);
			owner->addComponent(edge);
		}

		for (int y = sy; y <= fy; y += dy)
		{
			GridEdge* edge = new GridEdge();
			float z = 0.0f;
			if (y % 10 == 0)
			{
				edge->color = 0x80ffffff;
				z += 0.001f;
			}
			else
			{
				edge->color = 0x20404040;
			}

			edge->position = core::Vector3(c.getX(), y, z);
			edge->halfSize = core::Vector3(halfSize.getX(), core::Math::Epsilon, core::Math::Epsilon);
			edge->p0 = core::Vector3(c.getX() - halfSize.getX(), y, 0.0f);
			edge->p1 = core::Vector3(c.getX() + halfSize.getX(), y, 0.0f);
			owner->addComponent(edge);
		}
	}

	GridEdge::GridEdge(): color(0)
	{
		renderOrder = game::RenderOrder::Default;
	}

	void GridEdge::loaded(const game::LoadedArgs& e)
	{
	}

	void GridEdge::render(const game::RenderArgs& e)
	{
		game::Drawing::Default.pen.color = color;
		game::Drawing::Default.pen.width = e.scene->camera()->pixelSize;
		game::Drawing::Default.drawLine(p0, p1);
	}
}
