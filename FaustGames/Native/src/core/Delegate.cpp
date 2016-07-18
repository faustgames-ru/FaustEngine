#include "Delegate.h"

namespace core
{
	void DelegateVoid::clear()
	{
		for (uint i = 0; i < _callbacks.size(); i++)
			_callbacks[i]->dispose();
		_callbacks.clear();
	}

	void DelegateVoid::invoke()
	{
		for (uint i = 0; i < _callbacks.size(); i++)
			_callbacks[i]->invoke();
	}
}