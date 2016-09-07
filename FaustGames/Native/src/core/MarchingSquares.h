#ifndef MARCHING_SQUARES_H
#define MARCHING_SQUARES_H

#include "core_classes.h"
#include "llge.h"

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
		MarchingSquaresEdgeCase Cases[16];
		MarchingSquaresCases();
	};

	class MarchingSquares : public llge::IMarchingSquares
	{
	public:
		static MarchingSquaresCases DefaultCases;
		MarchingSquares();
		virtual void API_CALL build(IntPtr boolPoints, int w, int h) OVERRIDE;
		virtual void API_CALL buildSolid(IntPtr boolPoints, IntPtr boolInversePoints, int w, int h) OVERRIDE;
		virtual void API_CALL collectEdges() OVERRIDE;
		virtual void API_CALL simplifyPathes(int tolerance) OVERRIDE;
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

		virtual int API_CALL getSolidVerticesCount() OVERRIDE;
		virtual IntPtr API_CALL getSolidVertices() OVERRIDE;
		virtual int API_CALL getSolidIndicesCount() OVERRIDE;
		virtual IntPtr API_CALL getSolidIndices() OVERRIDE;
		virtual int API_CALL getBlendVerticesCount() OVERRIDE;
		virtual IntPtr API_CALL getBlendVertices() OVERRIDE;
		virtual int API_CALL getBlendIndicesCount() OVERRIDE;
		virtual IntPtr API_CALL getBlendIndices() OVERRIDE;


		virtual void API_CALL dispose() OVERRIDE;

		std::vector<MarchingSquaresPoint>& getTriangulationVertices();
		std::vector<ushort>& getTriangulationIndices();
	private:
		void collectEdges(std::vector<MarchingSquaresEdge>& edges, std::vector<std::vector<MarchingSquaresPoint>>& points);
		void simplifyPathes(int tolerance, std::vector<std::vector<MarchingSquaresPoint>>& points);
		void clipPathes(std::vector<std::vector<MarchingSquaresPoint>>& subj, std::vector<std::vector<MarchingSquaresPoint>>& clip, int type, std::vector<std::vector<MarchingSquaresPoint>>& result);
		void triangulatePathes(std::vector<std::vector<MarchingSquaresPoint>>& points, std::vector<MarchingSquaresPoint>& _vertices, std::vector<ushort>& _indices);
		static bool getValue(bool *points, int w, int h, int x, int y);
		static char getValue(bool l00, bool l01, bool l10, bool l11);
		void simplifyPath(std::vector<MarchingSquaresPoint> &path, int tolerance) const;
		void simplifyClosedPath(std::vector<MarchingSquaresPoint> &path, int tolerance) const;
		int _w;
		int _h;
		std::vector<MarchingSquaresEdge> _edges;
		std::vector<MarchingSquaresEdge> _inverseEdges;
		std::vector<std::vector<MarchingSquaresPoint> > _points;
		std::vector<std::vector<MarchingSquaresPoint> > _inversePoints;

		std::vector<ushort> _indices;
		std::vector<MarchingSquaresPoint> _vertices;

		std::vector<ushort> _solidIndices;
		std::vector<MarchingSquaresPoint> _solidVertices;
		std::vector<ushort> _blendIndices;
		std::vector<MarchingSquaresPoint> _blendVertices;
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
		MarchingSquaresNode();
		std::vector<MarchingSquaresSegment*> segments;
		std::vector<MarchingSquaresNode*> neighbours;
	};
}

#endif /*MARCHING_SQUARES_H*/
