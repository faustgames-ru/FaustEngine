#include "ZomboStateController.h"

namespace zombo
{
	void StateBase::activated()
	{
	}

	void StateBase::deactivated()
	{
	}

	void StateBase::update(float ellapsedTime)
	{
	}

	StateController::StateController() 
		: _actualState(nullptr), _newState(nullptr)
	{
	}

	void StateController::update(float ellapsedTime)
	{
		if (_newState != nullptr)
		{
			if (_actualState != nullptr)
			{
				_actualState->deactivated();
			}
			_actualState = _newState;
			if (_actualState != nullptr)
			{
				_actualState->activated();
			}
		}
		if (_actualState != nullptr)
		{
			_actualState->update(ellapsedTime);
		}
	}

	void StateController::setState(IState* state)
	{
		_newState = state;
	}
}
