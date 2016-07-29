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

	MeshesResult::MeshesResult(): _vertices(nullptr), _indices(nullptr)
	{
	}

	void MeshesResult::init(std::vector<core::MeshVertex>* vertices, std::vector<ushort>* indices)
	{
		_meshes.clear();
		_indices = indices;
		_vertices = vertices;
	}

	void MeshesResult::addMesh(core::MeshIndexer mesh)
	{
		_meshes.push_back(mesh);
	}

	int API_CALL MeshesResult::getMeshType(int meshIndex)
	{
		return _meshes[meshIndex].Type;
	}


	int MeshesResult::getMeshesCount()
	{
		return _meshes.size();
	}

	int MeshesResult::getVerticesCount(int meshIndex)
	{
		return _meshes[meshIndex].VerticesCount;
	}

	int MeshesResult::getIndicesCount(int meshIndex)
	{
		return _meshes[meshIndex].IndicesCount;
	}

	void MeshesResult::getVertices(int meshIndex, IntPtr vertices)
	{
		core::MeshVertex* ptr = static_cast<core::MeshVertex*>(vertices);
		core::MeshIndexer mesh = _meshes[meshIndex];
		for (uint i= 0, count = mesh.VerticesCount; i < count; i++)
		{
			ptr[i] = (*_vertices)[mesh.VerticesFirst + i];
		}
	}

	void MeshesResult::getIndices(int meshIndex, IntPtr indices)
	{
		ushort* ptr = static_cast<ushort*>(indices);
		core::MeshIndexer mesh = _meshes[meshIndex];
		for (uint i = 0, count = mesh.IndicesCount; i < count; i++)
		{
			ptr[i] = (*_indices)[mesh.IndicesFirst + i];
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

	const int MaxTileDetail = 64;
	float _txCoords[MaxTileDetail+1];
	float _tyCoords[MaxTileDetail+1];

	void TerrainClipper::build(int sizeX, int sizeY, int detailX, int detailY, bool createDifference)
	{
		if (detailX <= 0)
		{
			detailX = 1;
		}
		if (detailX > MaxTileDetail)
		{
			detailX = MaxTileDetail;
		}
		for (int i = 0; i <= detailX; i++)
		{
			_txCoords[i] = static_cast<float>(i) / static_cast<float>(detailX);
			_tyCoords[i] = 1.0f - static_cast<float>(i) / static_cast<float>(detailX);
		}

		sizeX /= detailX;
		sizeY /= detailX;
		_tileSizeX = sizeX*detailX;
		_tileSizeY = sizeY*detailX;
		_createDifference = createDifference;
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

		_intersectionResult.init(&_vertices, &_indices);
		for (uint i = 0; i < _meshesIntersection.size(); i++)
		{
			_intersectionResult.addMesh(_meshesIntersection[i].Indexer);
		}
		_differenceResult.init(&_vertices, &_indices);
		for (uint i = 0; i < _meshesDifference.size(); i++)
		{
			_differenceResult.addMesh(_meshesDifference[i].Indexer);
		}
	}

	void TerrainClipper::clearClipper()
	{
		clear();
	}

	void TerrainClipper::addClipperContour(IntPtr vertices2f, uint count)
	{
		addContour(static_cast<core::Vector2*>(vertices2f), count);
	}

	void TerrainClipper::buildClipper(int sizeX, int sizeY, int detailX, int detailY, bool createDifference)
	{
		build(sizeX, sizeY, detailX, detailY, createDifference);
	}

	llge::IMeshesResult* TerrainClipper::getIntersectionResult()
	{
		return &_intersectionResult;
	}

	llge::IMeshesResult* TerrainClipper::getDifferenceResult()
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
			mesh.Indexer.Type = 0;
			mesh.Indexer.VerticesFirst = _vertices.size();

			int ix = ((aabb.minX - _aabb.minX) / _sizeX) % (_tileSizeX / _sizeX);
			int iy = ((aabb.minY - _aabb.minY) / _sizeY) % (_tileSizeY / _sizeY);
			float u0 = _txCoords[ix];
			float v0 = _tyCoords[iy];
			float u1 = _txCoords[ix + 1];
			float v1 = _tyCoords[iy + 1];

			std::vector<p2t::Point> allPoints;
			for (uint i= 0; i < paths[j].size(); i++)
			{
				allPoints.push_back(p2t::Point(paths[j][i].X, paths[j][i].Y));
				float ut = static_cast<float>(paths[j][i].X - aabb.minX) / static_cast<float>(aabb.maxX - aabb.minX);
				float vt = static_cast<float>(paths[j][i].Y - aabb.minY) / static_cast<float>(aabb.maxY - aabb.minY);
				float u = core::Math::lerp(u0, u1, ut);
				float v = core::Math::lerp(v0, v1, vt);
				_vertices.push_back(core::MeshVertex(paths[j][i].X, paths[j][i].Y, 0, u, v, 0xffffffff));
			}
			mesh.Indexer.VerticesCount = allPoints.size();

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

			mesh.Indexer.IndicesFirst = _indices.size();
			
			for (uint i = 0; i < triangles.size(); i++)
			{
				p2t::Triangle *st = triangles[i];
				bool removeTria = false;
				uint t[3];
				for (uint k = 0; k < 3; k++)
				{
					p2t::Point *p = st->GetPoint(k);
					int n = p - baseArd;
					if (n < allPoints.size() && n >= 0)
					{
						t[k] = n;
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
			
			mesh.Indexer.IndicesCount = count;
			meshes.push_back(mesh);
		}
	}

	void TerrainClipper::createTile(const TerrainClipperAabb& aabb, std::vector<TerrainClipperMesh>& meshes)
	{
		TerrainClipperMesh mesh;
		mesh.aabb = aabb;
		mesh.Indexer.Type = 0;
		mesh.Indexer.VerticesFirst = _vertices.size();
		mesh.Indexer.VerticesCount = 4;
		mesh.Indexer.IndicesFirst = 0;
		mesh.Indexer.IndicesCount = 6;

		int ix = ((aabb.minX - _aabb.minX) / _sizeX) % (_tileSizeX / _sizeX);
		int iy = ((aabb.minY - _aabb.minY) / _sizeY) % (_tileSizeY / _sizeY);
		float u0 = _txCoords[ix];
		float v0 = _tyCoords[iy];
		float u1 = _txCoords[ix+1];
		float v1 = _tyCoords[iy+1];

		_vertices.push_back(core::MeshVertex(aabb.minX, aabb.minY, 0.0f, u0, v0, 0xffffffff));
		_vertices.push_back(core::MeshVertex(aabb.minX, aabb.maxY, 0.0f, u0, v1, 0xffffffff));
		_vertices.push_back(core::MeshVertex(aabb.maxX, aabb.maxY, 0.0f, u1, v1, 0xffffffff));
		_vertices.push_back(core::MeshVertex(aabb.maxX, aabb.minY, 0.0f, u1, v0, 0xffffffff));
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
		ClipperLib::SimplifyPolygons(intersection);
		if (_createDifference)
		{
			clipper.Execute(ClipperLib::ctDifference, difference);
			ClipperLib::SimplifyPolygons(difference);
		}

		if (aabb.hasSize(_sizeX, _sizeX))
		{
			if (!intersection.empty())
			{
				createTile(aabb, intersection, _meshesIntersection);
			}
			if (_createDifference)
			{
				if (!difference.empty())
				{
					createTile(aabb, difference, _meshesDifference);
				}
			}
			return;
		}

		if (aabb.equalArea(intersection))
		{
			createTiles(aabb, _meshesIntersection);
			return;
		}
		if (_createDifference)
		{
			if (aabb.equalArea(difference))
			{
				createTiles(aabb, _meshesDifference);
				return;
			}
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

	void P2t::buildContour(IntPtr vertices2f, uint vertivesCount)
	{
		core::Vector2* vertices = static_cast<core::Vector2*>(vertices2f);
		_allPoints.clear();
		_triangles.clear();
		for (uint i = 0; i < vertivesCount; i++)
		{
			_allPoints.push_back(p2t::Point(vertices[i].getX(), vertices[i].getY()));
		}

		std::vector<p2t::Point *> contour;
		for (uint i = 0; i < vertivesCount; i++)
		{
			bool hasEquals = false;
			for (uint j = 0; j < contour.size(); j++)
			{
				if (core::Vector2::equals(core::Vector2(_allPoints[i].x, _allPoints[i].y), core::Vector2(contour[j]->x, contour[j]->y)))
				{
					hasEquals = true;
					break;
				}
			}
			if (hasEquals)
				continue;
			contour.push_back(_allPoints.data() + i);
		}
		p2t::CDT cdt(contour);

		cdt.Triangulate();
		//todo: create triangles;
		std::vector<p2t::Triangle *> triangles = cdt.GetTriangles();
		p2t::Point *baseArd = _allPoints.data();
		int count = 0;

		for (uint i = 0; i < triangles.size(); i++)
		{
			p2t::Triangle *st = triangles[i];
			bool removeTria = false;
			uint t[3];
			for (uint k = 0; k < 3; k++)
			{
				p2t::Point *p = st->GetPoint(k);
				int n = p - baseArd;
				if (n < _allPoints.size() && n >= 0)
				{
					t[k] = n;
				}
				else
				{
					removeTria = true;
				}
			}
			if (!removeTria)
			{
				_triangles.push_back(t[0]);
				_triangles.push_back(t[1]);
				_triangles.push_back(t[2]);
				count += 3;
			}
		}
	}

	int P2t::getTrianglesCount()
	{
		return _triangles.size();
	}

	void P2t::getTriangles(IntPtr triangles)
	{
		int* result = static_cast<int*>(triangles);
		for (uint i = 0; i < _triangles.size(); i++)
			result[i] = _triangles[i];
	}
}