#include "llge.h"
#include "../core/core.h"
#include "../geometry/geometry.h"

#include <limits>
#include "../core/MarchingSquares.h"

using namespace geometry;
using namespace core;

namespace llge
{	
	class QuadTree : public IQuadTree
	{
	public:
		Quadtree2d Tree;
		FilterResult results;
		
		QuadTree()
		{
		}

		virtual int API_CALL insert(float minX, float minY, float maxX, float maxY, int userData)
		{
			Aabb2d aabb(minX, minY, maxX, maxY);
			return Tree.insert(aabb, userData);
		}
		
		virtual void API_CALL remove(int id)
		{
			return Tree.remove(id);
		}
		
		virtual void API_CALL query(float minX, float minY, float maxX, float maxY)
		{
			Aabb2d aabb(minX, minY, maxX, maxY);
			results.items.count = 0;
			results.iterations = 0;
			Tree.filt(aabb, results);
		}

		virtual void API_CALL getQueryResults(void *intBuffer)
		{
			memcpy(intBuffer, results.items.data, results.items.count * sizeof(int));
		}

		virtual int API_CALL getIterationsCount() 
		{
			return results.iterations;
		}

		
		virtual int API_CALL getQueryResultsCount()
		{
			return results.items.count;
		}

		virtual void API_CALL dispose()
		{
			delete this;
		}
	};

	class GeometryFactory : public IGeometryFactory
	{
	public:
		virtual IQuadTree * API_CALL createQuadTree()
		{
			return new QuadTree();
		}

		virtual IMarchingSquares* API_CALL createMarchingSquares()
		{
			return new MarchingSquares();
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