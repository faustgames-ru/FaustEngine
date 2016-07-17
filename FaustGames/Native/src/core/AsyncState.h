#ifndef CORE_ASYNC_STATE_H
#define CORE_ASYNC_STATE_H

#include "core_classes.h"
#include "Delegate.h"

namespace core
{
	class IAsyncState: IBaseObject
	{
		
	};
	
	template<typename T>
	class AsyncState
	{
	public:
		static AsyncState<T>* create();
		Delegate<T> callback;		
		void compleate(T result) const;
	private:
		AsyncState();
	};

	class AsyncChain
	{
	public:
		AsyncChain();
		Delegate<void> callback;
		template<typename T>
		void addState(AsyncState<T> state);
	private:
		template<typename T>
		void compleated();
		int _count;
	};

	inline AsyncChain::AsyncChain()
	{
		_count = 0;
	}

	template <typename T>
	AsyncState<T>* AsyncState<T>::create()
	{
		return new AsyncState<T>();
	}

	template <typename T>
	void AsyncState<T>::compleate(T result) const
	{
		callback.invoke(result);
		delete this;
	}

	template <typename T>
	AsyncState<T>::AsyncState()
	{
	}

	template <typename T>
	void AsyncChain::addState(AsyncState<T> state)
	{
		_count++;
		state.callback.add(this, &AsyncChain::compleated<T>);
	}

	template <typename T>
	void AsyncChain::compleated()
	{
		_count--;
		if (_count == 0)
		{
			callback.invoke();
			delete this;
		}
	}
}

#endif /*CORE_ASYNC_STATE_H*/