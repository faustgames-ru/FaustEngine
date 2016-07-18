#include "AsyncState.h"

namespace core
{
	AsyncChain::AsyncChain(): _count(0)
	{
		
	}

	AsyncChain* AsyncChain::create()
	{
		return new AsyncChain();
	}

	AsyncChain* AsyncChain::create(AsyncChain* state)
	{
		AsyncChain* result = new AsyncChain();
		result->addState(state);
		return result;
	}

	void AsyncChain::addState(AsyncChain* state)
	{		
		_count++;
		state->addCallback(this, &AsyncChain::compleated<AsyncStatus::e>);
	}
}