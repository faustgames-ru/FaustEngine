#ifndef ZOMBO_ASYNC_STATE_H
#define ZOMBO_ASYNC_STATE_H

#include "../zombo_classes.h"

namespace zombo
{
	template <typename TResult>
	typedef void (ZomboAsyncCallback)(TResult *result);
	
	template <typename TResult>
	class ZomboAsyncState
	{
	public:
		typedef ZomboAsyncCallback<TResult> Callback;
		
		void addCallback(Callback callback);
		void compleate(TResult result);
		bool isCompleate();
	private:
		std::vector<Callback> _callbacks;
	};
}

#endif /* ZOMBO_ASYNC_STATE_H */