#include "llge.h"
#include "../core/core.h"
#include "../geometry/geometry.h"

#include <limits>
#include "../core/MarchingSquares.h"
#include "../geometry/TerrainClipper.h"

using namespace geometry;
using namespace core;

namespace llge
{	
	class GeometryFactory : public IGeometryFactory
	{
	public:

		virtual IMarchingSquares* API_CALL createMarchingSquares()
		{
			return new MarchingSquares();
		}

		virtual ITerrainClipper* API_CALL createTerrainClipper()
		{
			return new TerrainClipper();
		}

		virtual IP2t * API_CALL createP2t()
		{
			return new P2t();
		}


		virtual void API_CALL dispose()
		{
			delete this;
		}
	};

	extern "C" DLLEXPORT  IGeometryFactory * API_CALL createGeometryFactory()
	{
		return new GeometryFactory();
	}
}