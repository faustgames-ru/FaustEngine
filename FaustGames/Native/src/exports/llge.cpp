// llge.cpp : Defines the exported functions for the DLL application.
//

#include "llge.h"
#include "../core/core.h"
#include "../graphics/graphics.h"
#include "../spine/SpineSkeletonResource.h"
#include "../navmesh/NavMesh.h"

#include <limits>

using namespace graphics;

namespace llge
{	
	extern "C" DLLEXPORT ISpineResource * API_CALL createSpineResource()
	{
        return new spine::SpineSkeletonResource();
	}
    
    
    extern "C" DLLEXPORT IPathMesh * API_CALL createPathMesh()
    {
        return new navmesh::PathMesh();
    }
    
    extern "C" DLLEXPORT INavMesh * API_CALL createNavMesh()
    {
        return new navmesh::NavMesh();
    }
    
    extern "C" DLLEXPORT INavMeshConfig * API_CALL createNavMeshConfig()
    {
        return new navmesh::NavMeshConfig();
    }
    
    extern "C" DLLEXPORT INavPath * API_CALL createNavPath()
    {
        return new navmesh::Path();
    }
}