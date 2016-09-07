#ifndef ZOMBO_ASYNC_STATE_H
#define ZOMBO_ASYNC_STATE_H

#include "../../zombo_classes.h"

namespace zombo
{
	typedef void (*ZomboAsyncCallback)();
	
	class ZomboAsyncState
	{
	public:
		typedef ZomboAsyncCallback Callback;
		
		void addCallback(Callback callback);
		void compleate();
		bool isCompleate();
	private:
		std::vector<Callback> _callbacks;
	};
}

#endif /* ZOMBO_ASYNC_STATE_H */