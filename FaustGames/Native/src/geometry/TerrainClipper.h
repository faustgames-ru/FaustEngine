#ifndef TERRAIN_CLIPPER_H
#define TERRAIN_CLIPPER_H

#include "geometry_classes.h"

#include "../src_clipper/clipper.hpp"
#include "Aabb2d.h"

namespace geometry
{
	struct TerrainClipperAabb
	{
		int minX;
		int minY;
		int maxX;
		int maxY;
		void getPath(ClipperLib::Path& path) const;
		void getPath(ClipperLib::IntPoint* path) const;
		void divide(int sizeYLimit, TerrainClipperAabb &left, TerrainClipperAabb &right) const;
		bool hasSize(int sizeX, int sizeY) const;
		bool equalArea(const ClipperLib::Paths& path) const;
		bool equalArea(const ClipperLib::Path& path) const;
	};

	struct TerrainMesh2dVertex
	{
		float x;
		float y;
		float z;
		uint color;
		float u;
		float v;
		TerrainMesh2dVertex();
		TerrainMesh2dVertex(float x, float y, float z, float u, float v);
		TerrainMesh2dVertex(float x, float y, float z, float u, float v, uint color);
	};


	class TerrainClipperMesh
	{
	public:
		TerrainClipperAabb aabb;
		core::MeshIndexer Indexer;
	};

	class MeshesResult : public llge::IMeshesResult
	{
	public:
		MeshesResult();
		void init(std::vector<core::MeshVertex>* vertices, std::vector<ushort>* indices);
		void addMesh(core::MeshIndexer mesh);
		int API_CALL getMeshType(int meshIndex) OVERRIDE;
		virtual int API_CALL getMeshesCount() OVERRIDE;
		virtual int API_CALL getVerticesCount(int meshIndex) OVERRIDE;
		virtual int API_CALL getIndicesCount(int meshIndex) OVERRIDE;
		virtual void API_CALL getVertices(int meshIndex, IntPtr vertices) OVERRIDE;
		virtual void API_CALL getIndices(int meshIndex, IntPtr indices) OVERRIDE;
	private:
		std::vector<core::MeshVertex>* _vertices;
		std::vector<ushort>* _indices;
		std::vector<core::MeshIndexer> _meshes;
	};

	class TerrainClipper : public llge::ITerrainClipper
	{
	public:
		TerrainClipper();
		void clear();
		void addContour(const core::Vector2* countour, uint count);
		void build(int sizeX, int sizeY, int detailX, int detailY, bool createDifference);

		virtual void API_CALL clearClipper() OVERRIDE;
		virtual void API_CALL addClipperContour(IntPtr vertices2f, uint count) OVERRIDE;
		virtual void API_CALL buildClipper(int sizeX, int sizeY, int detailX, int detailY, bool createDifference) OVERRIDE;
		virtual llge::IMeshesResult* API_CALL getIntersectionResult() OVERRIDE;
		virtual llge::IMeshesResult* API_CALL getDifferenceResult() OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
	private:
		void createTile(const TerrainClipperAabb &aabb, const ClipperLib::Paths paths, std::vector<TerrainClipperMesh> &meshes);
		void createTile(const TerrainClipperAabb &aabb, std::vector<TerrainClipperMesh> &meshes);
		void createTiles(const TerrainClipperAabb &aabb, std::vector<TerrainClipperMesh> &meshes);
		void divide(const TerrainClipperAabb &aabb, const ClipperLib::Paths &pathes);
		ClipperLib::Paths _pathes;
		Aabb2d _contoursBounds;
		TerrainClipperAabb _aabb;
		int _sizeX;
		int _sizeY;
		int _tileSizeX;
		int _tileSizeY;
		std::vector<core::MeshVertex> _vertices;
		std::vector<ushort> _indices;
		std::vector<TerrainClipperMesh> _meshesIntersection;
		std::vector<TerrainClipperMesh> _meshesDifference;
		MeshesResult _intersectionResult;
		MeshesResult _differenceResult;
		bool _createDifference;
	};
}

#endif /*TERRAIN_CLIPPER_H*/