#ifndef RAZOR_INPUT_CONTROLLERS_MOUSE_CONTROLLER_H
#define RAZOR_INPUT_CONTROLLERS_MOUSE_CONTROLLER_H

#include "../InputBroker.h"
#include "../../razor_classes.h"

namespace razor
{
	class MouseController : public InputController
	{
	public:
		virtual void update(const InputUpdateArgs& e) OVERRIDE;
	private:
	};
}

#endif /*RAZOR_INPUT_CONTROLLERS_MOUSE_CONTROLLER_H*/
