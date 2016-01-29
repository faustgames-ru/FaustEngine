#ifndef NAVMESH_H
#define NAVMESH_H

#include "navmesh_classes.h"

namespace navmesh
{
	class NavMeshPolygon
	{
	public:
	};

	class NavMeshTriangle;
	class NavMeshEdge;
	class NavMeshPoint;

	class NavMeshPoint
	{
	public:
		core::Vector2 position;
		NavMeshPoint();
		NavMeshPoint(const core::Vector2 &p);
	};

	class NavMeshTriangle
	{
	public:
		NavMeshPoint* points[3];
		NavMeshEdge* edges[3];
	};

	class NavMeshEdge
	{
	public:
		NavMeshPoint* points[2];
		NavMeshTriangle* triangles[2];
	};

	class NavMeshConfig : public llge::INavMeshConfig
	{
	public:
		NavMeshConfig();
		~NavMeshConfig();
		
		std::vector<std::vector<core::Vector2>> staticPolygons;
		geometry::Aabb2d bounds;
		float resolution;

		virtual void API_CALL clear();
		virtual void API_CALL addPolygon(IntPtr vertices2f, uint verticesCount);
		virtual IntPtr API_CALL getNativeInstance();
		virtual void API_CALL dispose();

	};

	class NavMesh : public llge::INavMesh
	{
	public:
		NavMesh();
		~NavMesh();
		
		void build(NavMeshConfig *config);
		NavMeshPolygon* addPolygon(const std::vector<core::Vector2> &points);
		void removePolygon(NavMeshPolygon *polygon);

		virtual void API_CALL buildNavMesh(llge::INavMeshConfig* config);
		virtual void API_CALL dispose();
		virtual int API_CALL getTriagnlesCount();
		virtual void API_CALL getTriagnles(IntPtr triangles3f);

	private:
		ClipperLib::IntPoint toClipper(const core::Vector2 &v) const;
		core::Vector2 toVector(const ClipperLib::IntPoint &v) const;
		core::Vector2 toVector(const p2t::Point &v) const;
		void buildRoot(ClipperLib::PolyNode *root);
		void createRootTriangles(ClipperLib::PolyNode *root);
		void createRootTrianglesWithHolesOffset(ClipperLib::PolyNode *root);
		void clear();
		std::vector<NavMeshTriangle* > _triangles;
		std::vector<NavMeshEdge* > _edges;
		std::vector<NavMeshPoint* > _points;
		float _minX;
		float _minY;
		float _maxX;
		float _maxY;
		float _resolution;
	};
}

#endif /*NAVMESH_H*/