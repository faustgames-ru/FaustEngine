#ifndef GEOMETRY_CLASSES_H
#define GEOMETRY_CLASSES_H

#include "geometry_platform.h"
#include "geometry_references.h"

namespace geometry
{

	class GeometryConstants
	{
	public:
		static const int ObjectsLimit = 16384;
	};


	typedef core::StaticArray<int, 16384> FilterResult;

	class Quadtree2dItemList;
}

#endif /*GEOMETRY_CLASSES_H*/
