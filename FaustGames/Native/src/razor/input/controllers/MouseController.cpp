#include "MouseController.h"
#include "../../../game/input/Mouse.h"

namespace razor
{
	void MouseController::update(const InputUpdateArgs& e)
	{
		e.broker->target = e.scene->camera()->unproject(game::Mouse::Default.windowX, game::Mouse::Default.windowY).toVector2();
		if (game::Mouse::Default.rightPresed())
		{
			e.broker->velocity = 1.0f;
		}
		else
		{
			e.broker->velocity = 0.0f;
		}
	}
}