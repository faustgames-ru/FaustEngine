#ifndef MARCHING_SQUARES_H
#define MARCHING_SQUARES_H

#include "core_classes.h"
#include <llge.h>

namespace core
{
	struct MarchingSquaresPoint
	{
		int x;
		int y;
		MarchingSquaresPoint();
		MarchingSquaresPoint(int x, int y);
		int getKey() const;
	};

	struct MarchingSquaresEdge
	{
		MarchingSquaresPoint p0;
		MarchingSquaresPoint p1;
		MarchingSquaresEdge();
		MarchingSquaresEdge(int x0, int y0, int x1, int y1);
		MarchingSquaresEdge operator + (MarchingSquaresEdge value) const;
		MarchingSquaresEdge operator + (MarchingSquaresPoint p) const;
	};

	struct MarchingSquaresEdgeCase
	{
		int count;
		MarchingSquaresEdge edges[2];
		MarchingSquaresEdgeCase();
		MarchingSquaresEdgeCase(int x00, int y00, int x01, int y01);
		MarchingSquaresEdgeCase(
			int x00, int y00, int x01, int y01,
			int x10, int y10, int x11, int y11);
	};

	class MarchingSquaresCases
	{
	public:
		MarchingSquaresEdgeCase Cases[15];
		MarchingSquaresCases();
	};

	class MarchingSquares : public llge::IMarchingSquares
	{
	public:
		static MarchingSquaresCases DefaultCases;
		MarchingSquares();
		virtual void API_CALL build(IntPtr boolPoints, int w, int h) OVERRIDE;
		virtual void API_CALL collectEdges() OVERRIDE;
		virtual void API_CALL simplifyPathes() OVERRIDE;
		virtual void API_CALL triangulatePathes() OVERRIDE;
		virtual IntPtr API_CALL getEdges() OVERRIDE;
		virtual int API_CALL getEdgesCount() OVERRIDE;
		virtual int API_CALL getPathesCount() OVERRIDE;
		virtual int API_CALL getPathCount(int pathIndex) OVERRIDE;
		virtual IntPtr API_CALL getPath(int pathIndex) OVERRIDE;

		virtual int API_CALL getVerticesCount() OVERRIDE;
		virtual IntPtr API_CALL getVertices() OVERRIDE;
		virtual int API_CALL getIndicesCount() OVERRIDE;
		virtual IntPtr API_CALL getIndices() OVERRIDE;

		virtual void API_CALL dispose() OVERRIDE;

		std::vector<MarchingSquaresPoint>& getTriangulationVertices();
		std::vector<ushort>& getTriangulationIndices();
	private:
		static bool getValue(bool *points, int w, int h, int x, int y);
		static char getValue(bool l00, bool l01, bool l10, bool l11);
		void simplifyPath(std::vector<MarchingSquaresPoint> &path) const;
		void simplifyClosedPath(std::vector<MarchingSquaresPoint> &path) const;
		int _w;
		int _h;
		std::vector<MarchingSquaresEdge> _edges;
		std::vector<std::vector<MarchingSquaresPoint> > _points;

		std::vector<ushort> _indices;
		std::vector<MarchingSquaresPoint> _vertices;
	};

	struct MarchingSquaresNode;
	struct MarchingSquaresSegment
	{
		MarchingSquaresEdge edge;
		MarchingSquaresNode* n0;
		MarchingSquaresNode* n1;
		MarchingSquaresNode* getNeighbour(MarchingSquaresNode* n) const;
	};

	struct MarchingSquaresNode
	{
		MarchingSquaresPoint point;
		bool parsed;
		std::vector<MarchingSquaresSegment*> segments;
		std::vector<MarchingSquaresNode*> neighbours;
	};
}

#endif /*MARCHING_SQUARES_H*/
