#include "TerrainClipper.h"
#include "../../src_poly2tri/common/shapes.h"
#include "../../src_poly2tri/sweep/cdt.h"

namespace geometry
{
	void TerrainClipperAabb::getPath(ClipperLib::Path& path) const
	{
		path.clear();
		path.resize(4);
		path[0] = ClipperLib::IntPoint(minX, minY);
		path[1] = ClipperLib::IntPoint(minX, maxY);
		path[2] = ClipperLib::IntPoint(maxX, maxY);
		path[3] = ClipperLib::IntPoint(maxX, minY);
	}

	void TerrainClipperAabb::getPath(ClipperLib::IntPoint* path) const
	{
		path[0] = ClipperLib::IntPoint(minX, minY);
		path[1] = ClipperLib::IntPoint(minX, maxY);
		path[2] = ClipperLib::IntPoint(maxX, maxY);
		path[3] = ClipperLib::IntPoint(maxX, minY);
	}

	void TerrainClipperAabb::divide(int sizeYLimit, TerrainClipperAabb& left, TerrainClipperAabb& right) const
	{
		float sizeX = maxX - minX;
		float sizeY = maxY - minY;
		if (sizeX > sizeY || sizeY <= sizeYLimit)
		{
			left.minX = minX;
			left.minY = minY;
			left.maxX = minX + sizeX / 2;
			left.maxY = maxY;

			right.minX = minX + sizeX / 2;
			right.minY = minY;
			right.maxX = maxX;
			right.maxY = maxY;
		}
		else
		{
			left.minX = minX;
			left.minY = minY;
			left.maxX = maxX;
			left.maxY = minY + sizeY / 2;

			right.minX = minX;
			right.minY = minY + sizeY / 2;
			right.maxX = maxX;
			right.maxY = maxY;
		}
	}

	bool TerrainClipperAabb::hasSize(int sizeX, int sizeY) const
	{
		return maxX - minX <= sizeX  && maxY - minY <= sizeY;
	}

	bool TerrainClipperAabb::equalArea(const ClipperLib::Paths& paths) const
	{
		if (paths.size() != 1)
			return false;
		return equalArea(paths[0]);
	}


	int Area(const ClipperLib::Path &poly)
	{
		int size = static_cast<int>(poly.size());
		if (size < 3) return 0;

		int a = 0;
		for (int i = 0, j = size - 1; i < size; ++i)
		{
			a += (poly[j].X + poly[i].X) * (poly[j].Y - poly[i].Y);
			j = i;
		}
		return -a / 2;
	}

	bool TerrainClipperAabb::equalArea(const ClipperLib::Path& path) const
	{
		int area = ClipperLib::Area(path);
		return area >= (maxX - minX)*(maxY - minY);
	}

	TerrainMesh2dVertex::TerrainMesh2dVertex(): x(0), y(0),	z(0), color(0xffffffff), u(0), v(0)
	{
	}

	TerrainMesh2dVertex::TerrainMesh2dVertex(float ix, float iy, float iz, float iu, float iv) : x(ix), y(iy), z(iz), color(0xffffffff), u(iu), v(iv)
	{
	}

	TerrainMesh2dVertex::TerrainMesh2dVertex(float ix, float iy, float iz, float iu, float iv, uint icolor) : x(ix), y(iy), z(iz), color(icolor), u(iu), v(iv)
	{
	}

	TerrainClipperResult::TerrainClipperResult(): 
		_vertices(nullptr),
		_indices(nullptr),
		_meshes(nullptr)
	{
	}

	void TerrainClipperResult::init(std::vector<TerrainMesh2dVertex>* vertices, std::vector<ushort>* indices, std::vector<TerrainClipperMesh>* meshes)
	{
		_vertices = vertices;
		_indices = indices;
		_meshes = meshes;
	}

	int TerrainClipperResult::getMeshesCount()
	{
		return _meshes->size();
	}

	int TerrainClipperResult::getVerticesCount(int meshIndex)
	{
		return (*_meshes)[meshIndex].VerticesCount;
	}

	int TerrainClipperResult::getIndicesCount(int meshIndex)
	{
		return (*_meshes)[meshIndex].IndicesCount;
	}

	void TerrainClipperResult::getVertices(int meshIndex, IntPtr vertices)
	{
		TerrainMesh2dVertex* ptr = static_cast<TerrainMesh2dVertex*>(vertices);
		TerrainClipperMesh mesh = (*_meshes)[meshIndex];
		for (uint i= 0, count = mesh.VerticesCount; i < count; i++)
		{
			ptr[i] = (*_vertices)[(*_meshes)[meshIndex].VerticesFirst + i];
		}
	}

	void TerrainClipperResult::getIndices(int meshIndex, IntPtr indices)
	{
		ushort* ptr = static_cast<ushort*>(indices);
		TerrainClipperMesh mesh = (*_meshes)[meshIndex];
		for (uint i = 0, count = mesh.IndicesCount; i < count; i++)
		{
			ptr[i] = (*_indices)[(*_meshes)[meshIndex].IndicesFirst + i];
		}
	}

	TerrainClipper::TerrainClipper() : _sizeX(1), _sizeY(1)
	{
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);
	}

	void TerrainClipper::clear()
	{
		_pathes.clear();
		_contoursBounds = Aabb2d();
		_indices.clear();
		_vertices.clear();
		_meshesDifference.clear();
		_meshesIntersection.clear();
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);
	}

	void TerrainClipper::addContour(const core::Vector2* countour, uint count)
	{
		_pathes.push_back(ClipperLib::Path(count));
		for (uint i = 0, len = _pathes.back().size(); i < len; i++)
		{
			_pathes.back()[i] = ClipperLib::IntPoint(static_cast<int>(countour[i].getX()), static_cast<int>(countour[i].getY()));
			_contoursBounds.expand(countour[i]);

		}
	}

	void TerrainClipper::build(int sizeX, int sizeY)
	{
		core::Vector2 center = (_contoursBounds.Max + _contoursBounds.Min)*0.5f;
		core::Vector2 size = _contoursBounds.Max - _contoursBounds.Min;
		int boundsX = static_cast<int>(core::Math::round(size.getX())) + 1;
		int boundsY = static_cast<int>(core::Math::round(size.getY())) + 1;
		int boundsCenterX = static_cast<int>(core::Math::round(center.getX()));
		int boundsCenterY = static_cast<int>(core::Math::round(center.getY()));

		int rootSizeX = sizeX;
		int rootSizeY = sizeY;

		if (rootSizeX == 0) return;
		if (rootSizeY == 0) return;

		while (rootSizeX < boundsX)
		{
			rootSizeX *= 2;
		}

		while (rootSizeY < boundsY)
		{
			rootSizeY *= 2;
		}

		_aabb.minX = boundsCenterX - rootSizeX / 2;
		_aabb.minY = boundsCenterY - rootSizeY / 2;
		_aabb.maxX = _aabb.minX + rootSizeX;
		_aabb.maxY = _aabb.minY + rootSizeY;
		_sizeX = sizeX;
		_sizeY = sizeY;

		ClipperLib::SimplifyPolygons(_pathes);
		divide(_aabb, _pathes);

		_intersectionResult.init(&_vertices, &_indices, &_meshesIntersection);
		_differenceResult.init(&_vertices, &_indices, &_meshesDifference);
	}

	void TerrainClipper::clearClipper()
	{
		clear();
	}

	void TerrainClipper::addClipperContour(IntPtr vertices2f, uint count)
	{
		addContour(static_cast<core::Vector2*>(vertices2f), count);
	}

	void TerrainClipper::buildClipper(int sizeX, int sizeY)
	{
		build(sizeX, sizeY);
	}

	llge::ITerrainClipperResult* TerrainClipper::getIntersectionResult()
	{
		return &_intersectionResult;
	}

	llge::ITerrainClipperResult* TerrainClipper::getDifferenceResult()
	{
		return &_differenceResult;
	}

	void TerrainClipper::dispose()
	{
		delete this;
	}

	void TerrainClipper::createTile(const TerrainClipperAabb& aabb, const ClipperLib::Paths paths, std::vector<TerrainClipperMesh> &meshes)
	{
		// todo: create intersection mesh using p2t
		for (uint j = 0; j < paths.size(); j++)
		{

			TerrainClipperMesh mesh;
			mesh.aabb = aabb;
			mesh.VerticesFirst = _vertices.size();

			std::vector<p2t::Point> allPoints;
			for (uint i= 0; i < paths[j].size(); i++)
			{
				allPoints.push_back(p2t::Point(paths[j][i].X, paths[j][i].Y));
				float u = static_cast<float>(paths[j][i].X - aabb.minX) / static_cast<float>(aabb.maxX - aabb.minX);
				float v = static_cast<float>(paths[j][i].Y - aabb.minY) / static_cast<float>(aabb.maxY - aabb.minY);
				_vertices.push_back(TerrainMesh2dVertex(paths[j][i].X, paths[j][i].Y, 0, u, v, 0xffffffff));
			}
			mesh.VerticesCount = allPoints.size();

			std::vector<p2t::Point *> contour;
			for (uint i = 0; i < paths[j].size(); i++)
			{
				contour.push_back(allPoints.data() + i);
			}
			p2t::CDT cdt(contour);

			cdt.Triangulate();
			//todo: create triangles;
			std::vector<p2t::Triangle *> triangles = cdt.GetTriangles();
			p2t::Point *baseArd = allPoints.data();
			int count = 0;

			mesh.IndicesFirst = _indices.size();

			for (uint i = 0; i < triangles.size(); i++)
			{
				p2t::Triangle *st = triangles[i];
				bool removeTria = false;
				uint t[3];
				for (uint k = 0; k < 3; k++)
				{
					p2t::Point *p = st->GetPoint(k);
					uint n = p - baseArd;
					if (n < allPoints.size())
					{
						t[k] = st->GetPoint(k) - baseArd;
					}
					else
					{
						removeTria = true;
					}
				}
				if (!removeTria)
				{
					_indices.push_back(t[0]);
					_indices.push_back(t[1]);
					_indices.push_back(t[2]);
					count += 3;
				}
			}
			mesh.IndicesCount = count;
			meshes.push_back(mesh);
		}
	}

	void TerrainClipper::createTile(const TerrainClipperAabb& aabb, std::vector<TerrainClipperMesh>& meshes)
	{
		TerrainClipperMesh mesh;
		mesh.aabb = aabb;
		mesh.VerticesFirst = _vertices.size();
		mesh.VerticesCount = 4;
		mesh.IndicesFirst = 0;
		mesh.IndicesCount = 6;

		_vertices.push_back(TerrainMesh2dVertex(aabb.minX, aabb.minY, 0, 0, 0));
		_vertices.push_back(TerrainMesh2dVertex(aabb.minX, aabb.maxY, 0, 0, 1));
		_vertices.push_back(TerrainMesh2dVertex(aabb.maxX, aabb.maxY, 0, 1, 1));
		_vertices.push_back(TerrainMesh2dVertex(aabb.maxX, aabb.minY, 0, 1, 0));
		meshes.push_back(mesh);
	}

	void TerrainClipper::createTiles(const TerrainClipperAabb& aabb, std::vector<TerrainClipperMesh>& meshes)
	{
		if (aabb.hasSize(_sizeX, _sizeY))
		{
			createTile(aabb, meshes);
		}
		else
		{
			TerrainClipperAabb aabb0;
			TerrainClipperAabb aabb1;
			aabb.divide(_sizeY, aabb0, aabb1);
			createTiles(aabb0, meshes);
			createTiles(aabb1, meshes);
		}
	}

	void TerrainClipper::divide(const TerrainClipperAabb& aabb, const ClipperLib::Paths& pathes)
	{
		ClipperLib::Path path;
		aabb.getPath(path);

		ClipperLib::Clipper clipper;
		clipper.AddPath(path, ClipperLib::ptSubject, true);
		clipper.AddPaths(pathes, ClipperLib::ptClip, true);

		ClipperLib::Paths intersection;
		ClipperLib::Paths difference;
		clipper.Execute(ClipperLib::ctIntersection, intersection);
		clipper.Execute(ClipperLib::ctDifference, difference);

		if (aabb.hasSize(_sizeX, _sizeX))
		{
			if (!intersection.empty())
			{
				createTile(aabb, intersection, _meshesIntersection);
			}
			if (!difference.empty())
			{
				createTile(aabb, difference, _meshesDifference);
			}
			return;
		}

		if (aabb.equalArea(intersection))
		{
			createTiles(aabb, _meshesIntersection);
			return;
		}
		
		if (aabb.equalArea(difference))
		{
			createTiles(aabb, _meshesDifference);
			return;
		}
		
		if (!intersection.empty())
		{
			TerrainClipperAabb aabb0;
			TerrainClipperAabb aabb1;
			aabb.divide(_sizeY, aabb0, aabb1);
			divide(aabb0, intersection);
			divide(aabb1, intersection);
		}
	}
}