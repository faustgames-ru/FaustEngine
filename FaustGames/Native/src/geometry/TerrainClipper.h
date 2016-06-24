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
		int VerticesFirst;
		int VerticesCount;
		int IndicesFirst;
		int IndicesCount;
	};

	class TerrainClipperResult: public llge::ITerrainClipperResult
	{
	public:
		TerrainClipperResult();
		void init(std::vector<TerrainMesh2dVertex>* vertices, std::vector<ushort>* indices, std::vector<TerrainClipperMesh>* meshes);
		virtual int API_CALL getMeshesCount() OVERRIDE;
		virtual int API_CALL getVerticesCount(int meshIndex) OVERRIDE;
		virtual int API_CALL getIndicesCount(int meshIndex) OVERRIDE;
		virtual void API_CALL getVertices(int meshIndex, IntPtr vertices) OVERRIDE;
		virtual void API_CALL getIndices(int meshIndex, IntPtr indices) OVERRIDE;
	private:
		std::vector<TerrainMesh2dVertex>* _vertices;
		std::vector<ushort>* _indices;
		std::vector<TerrainClipperMesh>* _meshes;
	};

	class TerrainClipper : public llge::ITerrainClipper
	{
	public:
		TerrainClipper();
		void clear();
		void addContour(const core::Vector2* countour, uint count);
		void build(int sizeX, int sizeY);

		virtual void API_CALL clearClipper() OVERRIDE;
		virtual void API_CALL addClipperContour(IntPtr vertices2f, uint count) OVERRIDE;
		virtual void API_CALL buildClipper(int sizeX, int sizeY) OVERRIDE;
		virtual llge::ITerrainClipperResult* API_CALL getIntersectionResult() OVERRIDE;
		virtual llge::ITerrainClipperResult* API_CALL getDifferenceResult() OVERRIDE;
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
		std::vector<TerrainMesh2dVertex> _vertices;
		std::vector<ushort> _indices;
		std::vector<TerrainClipperMesh> _meshesIntersection;
		std::vector<TerrainClipperMesh> _meshesDifference;
		TerrainClipperResult _intersectionResult;
		TerrainClipperResult _differenceResult;
	};
}

#endif /*TERRAIN_CLIPPER_H*/