#include "InputBroker.h"

namespace razor
{
	InputBroker InputBroker::Default;

	InputBroker::InputBroker()
	{
		target = core::Vector2::empty;
		velocity = 0;
	}

	void InputBroker::update(game::Scene* scene)
	{
		InputUpdateArgs e;
		e.scene = scene;
		e.broker = this;
		for (uint i = 0; i < _updateList.size(); i++)
		{
			_updateList[i]->update(e);
		}
	}

	void InputBroker::add(InputController* inputUpdate)
	{
		_updateList.push_back(inputUpdate);
	}
}