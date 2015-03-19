
#include "llge.h"

#include "../resources/ContentManager.h"

namespace llge
{
	extern "C" DLLEXPORT  IContentManager * API_CALL createContentManager()
	{
		return new resources::ContentManager();
	}
}