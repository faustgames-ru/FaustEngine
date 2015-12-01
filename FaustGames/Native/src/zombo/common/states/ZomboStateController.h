#ifndef ZOMBO_STATE_CONTROLLER_H
#define ZOMBO_STATE_CONTROLLER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class IState
	{
	public:
		virtual ~IState()
		{
		}

		virtual void activated() = 0;
		virtual void deactivated() = 0;
		virtual void update(float ellapsedTime) = 0;
	};
	
	class StateBase: public IState
	{
	public:
		virtual void activated() OVERRIDE;
		virtual void deactivated() OVERRIDE;
		virtual void update(float ellapsedTime) OVERRIDE;
	};

	class StateController
	{
	public:
		StateController();
		void update(float ellapsedTime);
		void setState(IState *state);
	private:
		IState * _actualState;
		IState * _newState;
	};
}

#endif /*ZOMBO_STATE_CONTROLLER_H*/
