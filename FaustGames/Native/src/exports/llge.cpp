// llge.cpp : Defines the exported functions for the DLL application.
//

#include "llge.h"
#include "../core/core.h"
#include "../graphics/graphics.h"
#include "../spine/SpineSkeletonResource.h"

#include <limits>

using namespace graphics;

namespace llge
{	
	extern "C" DLLEXPORT ISpineResource * API_CALL createSpineResource()
	{
        return new spine::SpineSkeletonResource();
	}
}