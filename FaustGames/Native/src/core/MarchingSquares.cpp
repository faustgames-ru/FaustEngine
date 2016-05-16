#pragma warning(disable:4996)

#include "MarchingSquares.h"
#include "psimpl.h"
#include "Math.h"
#include "../../src_poly2tri/sweep/cdt.h"
#include <iterator>

namespace core
{
	MarchingSquaresPoint::MarchingSquaresPoint(): x(0), y(0)
	{
	}

	MarchingSquaresPoint::MarchingSquaresPoint(int px, int py)
	{
		x = px;
		y = py;
	}

	int MarchingSquaresPoint::getKey() const
	{
		return (y << 16) + x;
	}

	MarchingSquaresEdge::MarchingSquaresEdge()
	{
	}

	MarchingSquaresEdge::MarchingSquaresEdge(int x0, int y0, int x1, int y1)
	{
		p0.x = x0;
		p0.y = y0;
		p1.x = x1;
		p1.y = y1;
	}

	MarchingSquaresEdge MarchingSquaresEdge::operator+(MarchingSquaresEdge v) const
	{
		return MarchingSquaresEdge(p0.x + v.p0.x, p0.y + v.p0.y, p1.x + v.p1.x, p1.y + v.p1.y);
	}

	MarchingSquaresEdge MarchingSquaresEdge::operator+(MarchingSquaresPoint p) const
	{
		return MarchingSquaresEdge(p0.x + p.x, p0.y + p.y, p1.x + p.x, p1.y + p.y);
	}

	MarchingSquaresEdgeCase::MarchingSquaresEdgeCase(): count(0)
	{
	}

	MarchingSquaresEdgeCase::MarchingSquaresEdgeCase(int x00, int y00, int x01, int y01)
	{
		count = 1;
		edges[0] = MarchingSquaresEdge(x00, y00, x01, y01);
	}

	MarchingSquaresEdgeCase::MarchingSquaresEdgeCase(int x00, int y00, int x01, int y01, int x10, int y10, int x11, int y11)
	{
		count = 2;
		edges[0] = MarchingSquaresEdge(x00, y00, x01, y01);
		edges[1] = MarchingSquaresEdge(x10, y10, x11, y11);
	}

	MarchingSquaresCases::MarchingSquaresCases()
	{
		Cases[0] = MarchingSquaresEdgeCase();
		Cases[1] = MarchingSquaresEdgeCase(-1, 0, 0, 1);
		Cases[2] = MarchingSquaresEdgeCase(0, 1, 1, 0);
		Cases[3] = MarchingSquaresEdgeCase(-1, 0, 1, 0);
		
		Cases[4] = MarchingSquaresEdgeCase(1, 0, 0, -1);
		Cases[5] = MarchingSquaresEdgeCase(0, -1, -1, 0, 0, 1, 1, 0);
		Cases[6] = MarchingSquaresEdgeCase(0, 1, 0, -1);
		Cases[7] = MarchingSquaresEdgeCase(0, -1, -1, 0);

		Cases[8] = Cases[7];
		Cases[9] = Cases[6];
		Cases[10] = MarchingSquaresEdgeCase(-1, 0, 0, 1, 1, 0, 0, -1);
		Cases[11] = Cases[4];

		Cases[12] = Cases[3];
		Cases[13] = Cases[2];
		Cases[14] = Cases[1];
		Cases[15] = Cases[0];
	}

	MarchingSquaresCases MarchingSquares::DefaultCases;

	MarchingSquares::MarchingSquares(): _w(0), _h(0)
	{
	}

	void MarchingSquares::build(IntPtr boolPoints, int w, int h)
	{
		bool* points = static_cast<bool*>(boolPoints);
		for (int y = 0; y <= h; y++)
		{
			for (int x = 0; x <= w; x++)
			{
				// make test;
				bool l00 = getValue(points, w, h, x + 0, y + 0);
				bool l01 = getValue(points, w, h, x + 1, y + 0);
				bool l10 = getValue(points, w, h, x + 0, y + 1);
				bool l11 = getValue(points, w, h, x + 1, y + 1);
				char value = getValue(l00, l01, l10, l11);
				MarchingSquaresEdgeCase edgeCase = DefaultCases.Cases[value];
				MarchingSquaresPoint p(x * 2, y * 2);
				for (int i = 0; i < edgeCase.count; i++)
				{
					_edges.push_back(edgeCase.edges[i] + p);
				}
			}
		}
	}

	typedef std::map<int, MarchingSquaresNode*> NodesMap;

	MarchingSquaresNode* findOrCreateNode(NodesMap &nodes, MarchingSquaresPoint p)
	{
		int key0 = p.getKey();
		NodesMap::iterator it0 = nodes.find(key0);
		MarchingSquaresNode* n0 = nullptr;
		if (it0 == nodes.end())
		{
			n0 = new MarchingSquaresNode();
			n0->point = p;
			n0->parsed = false;
			nodes[key0] = n0;
		}
		else
		{
			n0 = nodes[key0];
		}
		return n0;
	}

	void MarchingSquares::collectEdges()
	{
		std::vector<MarchingSquaresSegment *> segments;
		NodesMap nodes;
		for (uint i = 0; i < _edges.size(); i++)
		{
			MarchingSquaresNode* n0 = findOrCreateNode(nodes, _edges[i].p0);
			MarchingSquaresNode* n1 = findOrCreateNode(nodes, _edges[i].p1);
			MarchingSquaresSegment* seg = new MarchingSquaresSegment();
			seg->edge = _edges[i];
			seg->n0 = n0;
			seg->n1 = n1;
			n0->segments.push_back(seg);
			n0->neighbours.push_back(n1);
			n1->segments.push_back(seg);
			n1->neighbours.push_back(n0);
			segments.push_back(seg);
		}


		int cn2 = 0;
		int c2 = 0;

		if (nodes.size() == 0)
		{
			return;
		}

		for (NodesMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
		{
			if (it->second->neighbours.size() != 2)
			{
				// exception?
				cn2++;
			}
			else
			{
				c2++;
			}
		}

		while (!nodes.empty())
		{	
			int maxY = 0;
			MarchingSquaresNode *node = nodes.begin()->second;
			for (NodesMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
			{
				if (it->second->point.y > maxY)
				{
					node = it->second;
					maxY = it->second->point.y;
				}
			}
			
			_points.push_back(std::vector<MarchingSquaresPoint>());
			std::vector<MarchingSquaresPoint>& contour = _points.back();
			while (true)
			{
				node->parsed = true;
				contour.push_back(node->point);
				MarchingSquaresNode *next = nullptr;
				for (uint i = 0; i < node->neighbours.size(); i++)
				{
					if(!node->neighbours[i]->parsed)
					{
						next = node->neighbours[i];
						break;
					}
				}
				node = next;
				if (node == nullptr)
					break;
			}
			for (uint i = 0; i < contour.size(); i++)
			{
				NodesMap::iterator it = nodes.find(contour[i].getKey());
				nodes.erase(it);
			}
		}

		for (NodesMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
		{
			delete it->second;
		}
		for (uint i = 0; i < segments.size(); i++)
		{
			delete segments[i];
		}
	}

	int polygonArea(std::vector<MarchingSquaresPoint>& p)
	{
		int area = 0.;
		int points = p.size();
		int i, j = points - 1;
		for (i = 0; i < points; i++) 
		{
			area += (p[j].x + p[i].x)*(p[j].y - p[i].y); j = i;
		}

		return core::Math::abs(area*0.5);
	}

	void MarchingSquares::simplifyPathes(int tolerance)
	{
		/*
		int maxArea = 0;
		int maxIndex = -1;
		for (uint i = 0; i < _points.size(); i++)
		{
			int area = polygonArea(_points[i]);
			if (area > maxArea)
			{
				maxArea = area;
				maxIndex = i;
			}
		}

		if (maxIndex >= 0)
		{
			std::vector<MarchingSquaresPoint> path = _points[maxIndex];
			simplifyClosedPath(path);
			_points.clear();
			_points.push_back(path);
		}
		*/
		for (uint i = 0; i < _points.size(); i++)
		{
			simplifyClosedPath(_points[i], tolerance);
		}		
	}

	void MarchingSquares::triangulatePathes()
	{
		if (_points.size() == 0) return;
		std::vector<std::vector<uint> > polylinesIndices;
		std::vector<std::vector<p2t::Point *> > polylines;
		std::vector<p2t::Point> allPoints;		
		_vertices.clear();

		polylines.resize(_points.size());
		polylinesIndices.resize(_points.size());

		for (uint j = 0; j < polylines.size(); j++)
		{
			for (uint k = 0; k < _points[j].size(); k++)
			{
				uint i = allPoints.size();
				allPoints.push_back(p2t::Point(_points[j][k].x, _points[j][k].y));
				_vertices.push_back(_points[j][k]);
				polylinesIndices[j].push_back(i);
			}
		}
		if (allPoints.size() == 0) return;
		for (uint j = 0; j < polylines.size(); j++)
		{
			polylines[j].resize(polylinesIndices[j].size());
			for (uint k = 0; k < polylinesIndices[j].size(); k++)
			{
				polylines[j][k] = &(allPoints[polylinesIndices[j][k]]);
			}
		}

		if (polylines.size() == 0) return;

		p2t::CDT cdt(polylines.back());
		for (uint k = 0; k < (polylines.size() - 1); k++)
		{
			if (polylines[k].size() == 0) continue;
			cdt.AddHole(polylines[k]);
		}
		cdt.Triangulate();
		std::vector<p2t::Triangle *> triangles = cdt.GetTriangles();
		p2t::Point *baseArd = &allPoints[0];

		for (uint j = 0; j < triangles.size(); j++)
		{
			p2t::Point *p0 = triangles[j]->GetPoint(0);
			p2t::Point *p1 = triangles[j]->GetPoint(1);
			p2t::Point *p2 = triangles[j]->GetPoint(2);

			ushort i0 = p0 - baseArd;
			ushort i1 = p1 - baseArd;
			ushort i2 = p2 - baseArd;
			if (i0 >= allPoints.size())
				continue;
			if (i1 >= allPoints.size())
				continue;
			if (i2 >= allPoints.size())
				continue;
			_indices.push_back(i0);
			_indices.push_back(i1);
			_indices.push_back(i2);
		}
	}

	IntPtr MarchingSquares::getEdges()
	{
		return _edges.data();
	}

	int MarchingSquares::getEdgesCount()
	{
		return _edges.size();
	}

	int MarchingSquares::getPathesCount()
	{
		return _points.size();
	}

	int MarchingSquares::getPathCount(int pathIndex)
	{
		return _points[pathIndex].size();
	}

	IntPtr MarchingSquares::getPath(int pathIndex)
	{
		return _points[pathIndex].data();
	}

	int MarchingSquares::getVerticesCount()
	{		
		return _vertices.size();
	}

	IntPtr MarchingSquares::getVertices()
	{
		return _vertices.data();
	}

	int MarchingSquares::getIndicesCount()
	{
		return _indices.size();
	}

	IntPtr MarchingSquares::getIndices()
	{
		return _indices.data();
	}

	void MarchingSquares::dispose()
	{
		delete this;
	}

	std::vector<MarchingSquaresPoint>& MarchingSquares::getTriangulationVertices()
	{
		return _vertices;
	}

	std::vector<ushort>& MarchingSquares::getTriangulationIndices()
	{
		return _indices;
	}

	bool MarchingSquares::getValue(bool* points, int w, int h, int x, int y)
	{
		if (x == 0)
			return false;
		if (y == 0)
			return false;
		x--;
		y--;
		if (x == w)
			return false;
		if (y == h)
			return false;
		return points[y*w + x];
	}

	char MarchingSquares::getValue(bool l00, bool l01, bool l10, bool l11)
	{
		char bit0 = l10 ? 1 : 0;
		char bit1 = l11 ? 1 : 0;
		char bit2 = l01 ? 1 : 0;
		char bit3 = l00 ? 1 : 0;
		return bit0 + (bit1 << 1) + (bit2 << 2) + (bit3 << 3);
	}

	void MarchingSquares::simplifyPath(std::vector<MarchingSquaresPoint>& path, int tolerance) const
	{
		std::vector<int> polyline;  
		std::vector<int> result0;
		std::vector<int> result1;

		for (uint i = 0; i < path.size(); i++)
		{
			polyline.push_back(path[i].x);
			polyline.push_back(path[i].y);
		}

		psimpl::simplify_douglas_peucker<2>(polyline.begin(), polyline.end(), tolerance, std::back_inserter(result0));
		psimpl::simplify_radial_distance<2>(result0.begin(), result0.end(), tolerance, std::back_inserter(result1));

		path.clear();
		for (uint i = 0; i < result1.size()/2; i++)
		{
			MarchingSquaresPoint p;
			p.x = static_cast<int>(result1[i * 2 + 0]);
			p.y = static_cast<int>(result1[i * 2 + 1]);
			path.push_back(p);
		}
	}

	void MarchingSquares::simplifyClosedPath(std::vector<MarchingSquaresPoint>& path, int tolerance) const
	{
		
		if (polygonArea(path) < 32 * 32)
		{
			path.clear();
			return;
		}
		
		int farIndex = path.size() - 1;
		float maxL = 0;
		for (uint j = 1; j < path.size(); j++)
		{
			float dx = static_cast<float>(path[j].x - path[0].x);
			float dy = static_cast<float>(path[j].y - path[0].y);
			float l = dx*dx + dy*dy;
			if (l > maxL)
			{
				maxL = l;
				farIndex = j;
			}
		}

		std::vector<MarchingSquaresPoint> start;
		int j;
		for (j = 0; j <= farIndex; j++)
		{
			start.push_back(path[j]);
		}
		std::vector<MarchingSquaresPoint> finish;
		for (j = farIndex; j < static_cast<int>(path.size()) - 1; j++)
		{
			finish.push_back(path[j]);
		}
		finish.push_back(path[0]);
		simplifyPath(start, tolerance);
		simplifyPath(finish, tolerance);

		path.clear();
		for (j = 0; j < static_cast<int>(start.size()); j++)
		{
			path.push_back(start[j]);
		}
		for (j = 1; j < static_cast<int>(finish.size()) - 1; j++)
		{
			path.push_back(finish[j]);
		}

		if (polygonArea(path) < 32 * 32)
		{
			path.clear();
			return;
		}
	}

	MarchingSquaresNode* MarchingSquaresSegment::getNeighbour(MarchingSquaresNode* n) const
	{
		if (n == n0)
			return n1;
		if (n == n1)
			return n0;
		return nullptr;
	}

	MarchingSquaresNode::MarchingSquaresNode() : parsed(false)
	{
	}
}
