#ifndef CORE_ASYNC_STATE_H
#define CORE_ASYNC_STATE_H

#include "core_classes.h"
#include "Delegate.h"

namespace core
{
	struct AsyncStatus
	{
		enum e
		{
			None,			
		};
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
		static AsyncChain* create();
		static AsyncChain* create(AsyncChain* state);
		template<typename T>
		static AsyncChain* create(AsyncState<T>* state);
		template<typename T>
		void addState(AsyncState<T>* state);
		void addState(AsyncChain* state);

		template <typename TDelegate>
		void addCallback(TDelegate* delegateInstance, typename DelegateCallback<TDelegate, AsyncStatus::e>::Callback delegateMethod);
	private:
		AsyncChain();
		template<typename T>
		void compleated(T e);
		int _count;
		Delegate<AsyncStatus::e> _callback;
	};

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
	AsyncChain* AsyncChain::create(AsyncState<T>* state)
	{
		AsyncChain* result = new AsyncChain();
		result->addState(state);
		return result;
	}

	template <typename T>
	void AsyncChain::addState(AsyncState<T>* state)
	{
		_count++;
		state->callback.add(this, &AsyncChain::compleated<T>);
	}

	template <typename TDelegate>
	void AsyncChain::addCallback(TDelegate* delegateInstance, typename DelegateCallback<TDelegate, AsyncStatus::e>::Callback delegateMethod)
	{
		_callback.add(delegateInstance, delegateMethod);		
	}

	template <typename T>
	void AsyncChain::compleated(T e)
	{
		_count--;
		if (_count == 0)
		{
			_callback.invoke(e);
			delete this;
		}
	}
}

#endif /*CORE_ASYNC_STATE_H*/