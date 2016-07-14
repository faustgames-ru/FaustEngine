#ifndef CORE_DELEGATE_H
#define CORE_DELEGATE_H

#include "core_classes.h"

namespace core
{
	template <typename TArgs>
	class IDelegateCallback : public IBaseObject
	{
	public:
		virtual void invoke(TArgs e) = 0;
		virtual void dispose() = 0;
	};

	template <typename TDelegate, typename TArgs>
	class DelegateCallback: public IDelegateCallback<TArgs>
	{
	public:
		typedef void(TDelegate::*Callback)(TArgs e);
		TDelegate* delegateInstance;
		Callback delegateMethod;
		DelegateCallback(TDelegate* delegateInstance, Callback delegateMethod);
		virtual void invoke(TArgs e) OVERRIDE;
		virtual void dispose() OVERRIDE;
	};

	template <typename TArgs>
	class Delegate
	{
	public:
		template <typename TDelegate>
		void add(TDelegate* delegateInstance, typename DelegateCallback<TDelegate, TArgs>::Callback delegateMethod);
		void clear();
		void invoke(TArgs e);
	private:
		std::vector<IDelegateCallback<TArgs>*> _callbacks;
	};

	template <typename TDelegate, typename TArgs>
	DelegateCallback<TDelegate, TArgs>::DelegateCallback(TDelegate* instance, Callback method): delegateInstance(instance), delegateMethod(method)
	{		
	}

	template <typename TDelegate, typename TArgs>
	void DelegateCallback<TDelegate, TArgs>::invoke(TArgs e)
	{
		(delegateInstance->*delegateMethod)(e);
	}

	template <typename TDelegate, typename TArgs>
	void DelegateCallback<TDelegate, TArgs>::dispose()
	{
		delete this;
	}

	template <typename TArgs>
	template <typename TDelegate>
	void Delegate<TArgs>::add(TDelegate* delegateInstance, typename DelegateCallback<TDelegate, TArgs>::Callback delegateMethod)
	{
		_callbacks.push_back(new DelegateCallback<TDelegate, TArgs>(delegateInstance, delegateMethod));
	}

	template <typename TArgs>
	void Delegate<TArgs>::clear()
	{
		for (uint i = 0; i < _callbacks.size(); i++)
			_callbacks[i]->dispose();
		_callbacks.clear();
	}

	template <typename TArgs>
	void Delegate<TArgs>::invoke(TArgs e)
	{
		for (uint i = 0; i < _callbacks.size(); i++)
			_callbacks[i]->invoke(e);
	}
}

#endif /*CORE_DELEGATE_H*/