#include "NavMesh.h"


namespace navmesh
{
	NavMeshPoint::NavMeshPoint()
	{
	}

	NavMeshPoint::NavMeshPoint(const core::Vector2& p)
	{
		position = p;
	}

	NavMeshConfig::NavMeshConfig(): resolution(1)
	{
	}

	NavMeshConfig::~NavMeshConfig()
	{
	}

	void NavMeshConfig::clear()
	{
		staticPolygons.clear();
		bounds = geometry::Aabb2d();
	}

	void NavMeshConfig::addPolygon(IntPtr vertices2f, uint verticesCount)
	{
		staticPolygons.push_back(std::vector<core::Vector2>());
		std::vector<core::Vector2> &p = staticPolygons.back();
		core::Vector2* verts = static_cast<core::Vector2*>(vertices2f);
		for (uint i = 0; i < verticesCount; i++)
		{
			p.push_back(verts[i]);
			bounds.expand(verts[i]);
		}
	}

	IntPtr NavMeshConfig::getNativeInstance()
	{
		return this;
	}

	void NavMeshConfig::dispose()
	{
		delete this;
	}

	NavMesh::NavMesh() : _minX(0), _minY(0), _maxX(0), _maxY(0), _resolution(1)
	{
	}

	NavMesh::~NavMesh()
	{
		clear();
	}

	void NavMesh::build(NavMeshConfig* config)
	{
		core::Vector2 size = config->bounds.Max - config->bounds.Min;
		_minX = config->bounds.Min.getX() - size.getX() * 0.25f;
		_minY = config->bounds.Min.getY() - size.getY() * 0.25f;
		_maxX = config->bounds.Max.getX() + size.getX() * 0.25f;
		_maxY = config->bounds.Max.getY() + size.getY() * 0.25f;
		_resolution = config->resolution;

		std::vector<std::vector<core::Vector2>> &staticPolygons = config->staticPolygons;
		clear();

		ClipperLib::Clipper clipper;
		ClipperLib::Path pRoot;
		pRoot.push_back(toClipper(core::Vector2(_minX, _minY)));
		pRoot.push_back(toClipper(core::Vector2(_minX, _maxY)));
		pRoot.push_back(toClipper(core::Vector2(_maxX, _maxY)));
		pRoot.push_back(toClipper(core::Vector2(_maxX, _minY)));
		
		clipper.AddPath(pRoot, ClipperLib::ptClip, true);
		for (uint i = 0; i < staticPolygons.size(); i++)
		{
			const std::vector<core::Vector2> &poly = staticPolygons[i];
			ClipperLib::Path p;
			ClipperLib::ClipperOffset offset;
			for (uint j = 0; j < poly.size(); j++)
			{
				p.push_back(toClipper(poly[j]));
			}
			ClipperLib::Paths offPath;
			SimplifyPolygon(p, offPath);
			for (uint j = 0; j < offPath.size(); j++)
			{
				if (!ClipperLib::Orientation(offPath[j]))
				{
					ClipperLib::ReversePath(offPath[j]);
				}
				clipper.AddPath(offPath[j], ClipperLib::ptClip, true);
			}
		}

		ClipperLib::PolyTree solution = ClipperLib::PolyTree();
		clipper.Execute(ClipperLib::ctXor, solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

		ClipperLib::PolyNode *root = solution.GetFirst();
		for (int i = 0; i < solution.ChildCount(); i++)
		{
			buildRoot(solution.Childs[i]);
		}


	}

	NavMeshPolygon* NavMesh::addPolygon(const std::vector<core::Vector2>& points)
	{
		// todo polygon creation
		return nullptr;
	}

	void NavMesh::removePolygon(NavMeshPolygon* polygon)
	{
		// todo polygon removing
	}

	void NavMesh::buildNavMesh(llge::INavMeshConfig* config)
	{
		build(static_cast<NavMeshConfig*>(config->getNativeInstance()));
	}

	void NavMesh::dispose()
	{
		delete this;
	}

	int NavMesh::getTriagnlesCount()
	{
		return _triangles.size();
	}

	void NavMesh::getTriagnles(IntPtr triangles3f)
	{
		core::Vector2 *verts = static_cast<core::Vector2 *>(triangles3f);
		for (uint i = 0; i < _triangles.size(); i++)
		{

			for (uint j = 0; j < 3; j++)
			{
				*verts = _triangles[i]->points[j]->position;
				verts++;
			}
		}
	}

	ClipperLib::IntPoint NavMesh::toClipper(const core::Vector2& v) const
	{
		ClipperLib::IntPoint result;
		result.X = static_cast<ClipperLib::cInt>(core::Math::round((v.getX() - _minX) * _resolution));
		result.Y = static_cast<ClipperLib::cInt>(core::Math::round((v.getY() - _minY) * _resolution));
		return result;
	}

	core::Vector2 NavMesh::toVector(const ClipperLib::IntPoint& v) const
	{
		return core::Vector2(v.X / _resolution + _minX, v.Y / _resolution + _minY);
	}

	core::Vector2 NavMesh::toVector(const p2t::Point& v) const
	{
		return core::Vector2(
			static_cast<float>(v.x / _resolution + _minX),
			static_cast<float>(v.y / _resolution + _minY));
	}

	void NavMesh::buildRoot(ClipperLib::PolyNode* root)
	{
		bool isHole = root->IsHole();

		createRootTriangles(root);
		//createRootTrianglesWithHolesOffset(root);

		for (int i = 0; i < root->ChildCount(); i++)
		{
			ClipperLib::PolyNode *hole = root->Childs[i];
			for (int j = 0; j < hole->ChildCount(); j++)
			{
				buildRoot(hole->Childs[j]);
			}
		}
	}


	int find(const std::vector<ClipperLib::IntPoint> &points, const ClipperLib::IntPoint &p)
	{
		for (uint i = 0; i < points.size(); i++)
		{
			if (points[i] == p)
			{
				return i;
			}
		}
		return -1;
	}

	void NavMesh::createRootTriangles(ClipperLib::PolyNode* root)
	{
		std::vector<p2t::Point> allPoints;
		std::vector<ClipperLib::IntPoint> allPointsSources;

		std::vector<int> contourIndices;
		std::vector<std::vector<int>> holesIndices;
		for (uint i = 0; i < root->Contour.size(); i++)
		{
			int n = -1;//find(allPointsSources, root->Contour[i]);
			if (n < 0)
			{
				n = allPointsSources.size();
				allPointsSources.push_back(root->Contour[i]);
			}
			contourIndices.push_back(n);
		}

		holesIndices.resize(root->ChildCount());
		for (uint j = 0; j < root->ChildCount(); j++)
		{
			ClipperLib::PolyNode *hole = root->Childs[j];
			std::vector<int> &holeIndices = holesIndices[j];
			for (uint i = 0; i <  hole->Contour.size(); i++)
			{
				int n = -1;// find(allPointsSources, hole->Contour[i]);
				if (n < 0)
				{
					n = allPointsSources.size();
					allPointsSources.push_back(hole->Contour[i]);
				}
				holeIndices.push_back(n);
			}
		}
		for (uint i = 0; i < allPointsSources.size(); i++)
		{
			allPoints.push_back(p2t::Point(
				static_cast<double>(allPointsSources[i].X),
				static_cast<double>(allPointsSources[i].Y)));
		}

		std::vector<p2t::Point *> contour;
		std::vector<std::vector<p2t::Point *>> holes;
		
		for (uint i = 0; i < contourIndices.size(); i++)
		{
			p2t::Point* p = allPoints.data() + contourIndices[i];
			contour.push_back(p);
		}
		p2t::CDT cdt(contour);
		holes.resize(holesIndices.size());
		for (int i = 0; i < holesIndices.size(); i++)
		{
			ClipperLib::PolyNode *hole = root->Childs[i];
			std::vector<p2t::Point *> &contourHole = holes[i];
			for (uint j = 0; j < holesIndices[i].size(); j++)
			{
				p2t::Point* p = allPoints.data() + holesIndices[i][j];
				contourHole.push_back(p);
			}
			cdt.AddHole(holes[i]);
		}

		cdt.Triangulate();
		//todo: create triangles;
		std::vector<p2t::Triangle *> triangles = cdt.GetTriangles();
		p2t::Point *baseArd = &allPoints[0];

		uint baseIndex = _points.size();
		for (uint i = 0; i < allPoints.size(); i++)
		{
			_points.push_back(new NavMeshPoint(toVector(allPoints[i])));
		}
		for (uint i = 0; i < triangles.size(); i++)
		{
			NavMeshTriangle* t = new NavMeshTriangle();
			p2t::Triangle *st = triangles[i];
			bool removeTria = false;
			for (uint j = 0; j < 3; j++)
			{
				p2t::Point *p = st->GetPoint(j);
				uint n = p - baseArd;
				if (n < _points.size())
				{
					t->points[j] = _points[baseIndex + (st->GetPoint(j) - baseArd)];
				}
				else
				{
					removeTria = true;
				}
			}
			if (removeTria)
			{
				// log error
				delete t;
			}
			else
			{
				_triangles.push_back(t);
			}
		}
	}

	void NavMesh::createRootTrianglesWithHolesOffset(ClipperLib::PolyNode* root)
	{
		ClipperLib::ClipperOffset offset;
		offset.AddPath(root->Contour, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
		ClipperLib::Paths offsetContours;
		offset.Execute(offsetContours, 0.0);

		std::vector<p2t::Point> allPoints;
		std::vector<int> contourIndices;
		std::vector<std::vector<int>> holesIndices;

		if (offsetContours.size() != 1)
		{
			// log error
			return;
		}
		else
		{
			for (uint i = 0; i < offsetContours[0].size(); i++)
			{
				contourIndices.push_back(allPoints.size());
				allPoints.push_back(p2t::Point(
					static_cast<double>(offsetContours[0][i].X),
					static_cast<double>(offsetContours[0][i].Y)));
			}
		}

		holesIndices.resize(root->ChildCount());
		for (uint j = 0; j < root->ChildCount(); j++)
		{
			ClipperLib::PolyNode *hole = root->Childs[j];
			std::vector<int> &holeIndices = holesIndices[j];
			ClipperLib::ClipperOffset offsetHole;
			offsetHole.AddPath(hole->Contour, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
			ClipperLib::Paths res;
			offsetHole.Execute(res, -0.0);

			if (res.size() != 1)
			{
				// log error
				return;
			}

			for (uint i = 0; i < res[0].size(); i++)
			{
				holeIndices.push_back(allPoints.size());
				allPoints.push_back(p2t::Point(
					static_cast<double>(res[0][i].X),
					static_cast<double>(res[0][i].Y)));
			}
		}
		std::vector<p2t::Point *> contour;
		std::vector<std::vector<p2t::Point *>> holes;

		for (uint i = 0; i < contourIndices.size(); i++)
		{
			p2t::Point* p = allPoints.data() + contourIndices[i];
			contour.push_back(p);
		}
		p2t::CDT cdt(contour);
		holes.resize(holesIndices.size());
		for (int i = 0; i < holesIndices.size(); i++)
		{
			ClipperLib::PolyNode *hole = root->Childs[i];
			std::vector<p2t::Point *> &contourHole = holes[i];
			for (uint j = 0; j < holesIndices[i].size(); j++)
			{
				p2t::Point* p = allPoints.data() + holesIndices[i][j];
				contourHole.push_back(p);
			}
			cdt.AddHole(holes[i]);
		}

		cdt.Triangulate();
		//todo: create triangles;
		std::vector<p2t::Triangle *> triangles = cdt.GetTriangles();
		p2t::Point *baseArd = &allPoints[0];

		uint baseIndex = _points.size();
		for (uint i = 0; i < allPoints.size(); i++)
		{
			_points.push_back(new NavMeshPoint(toVector(allPoints[i])));
		}
		for (uint i = 0; i < triangles.size(); i++)
		{
			NavMeshTriangle* t = new NavMeshTriangle();
			p2t::Triangle *st = triangles[i];
			bool removeTria = false;
			for (uint j = 0; j < 3; j++)
			{
				p2t::Point *p = st->GetPoint(j);
				uint n = p - baseArd;
				if (n < _points.size())
				{
					t->points[j] = _points[baseIndex + (st->GetPoint(j) - baseArd)];
				}
				else
				{
					removeTria = true;
				}
			}
			if (removeTria)
			{
				// log error
				delete t;
			}
			else
			{
				_triangles.push_back(t);
			}
		}
	}

	void NavMesh::clear()
	{
		for (uint i = 0; i < _points.size(); i++)
			delete _points[i];

		_points.clear();
		for (uint i = 0; i < _edges.size(); i++)
			delete _edges[i];
		_edges.clear();

		for (uint i = 0; i < _triangles.size(); i++)
			delete _triangles[i];
		_triangles.clear();
	}
}

namespace llge
{
	extern "C" DLLEXPORT INavMesh * API_CALL createNavMesh()
	{
		return new navmesh::NavMesh();
	}

	extern "C" DLLEXPORT INavMeshConfig * API_CALL createNavMeshConfig()
	{
		return new navmesh::NavMeshConfig();
	}
}