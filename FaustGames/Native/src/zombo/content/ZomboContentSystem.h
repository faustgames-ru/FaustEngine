#ifndef ZOMBO_CONTENT_SYSTEM_H
#define ZOMBO_CONTENT_SYSTEM_H

#include "../zombo_classes.h"
#include "ZomboContentBlock.h"

namespace zombo
{	
	class ZomboContentSystem
	{
	public:
		//ZomboAsyncState* load(ZomboContentBlock *contentBlock);
		//ZomboAsyncState* unload(ZomboContentBlock *contentBlock);
		void update();
	private:
	};
}

#endif /* ZOMBO_CONTENT_SYSTEM_H */