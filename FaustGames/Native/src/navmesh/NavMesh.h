#ifndef NAVMESH_H
#define NAVMESH_H

#include "navmesh_classes.h"
#include "AStar.h"

namespace navmesh
{
	class NavMeshPolygon
	{
	public:
	};

	class NavMeshTriangle;
	class NavMeshEdge;
	class NavMeshPoint;

	typedef std::list<NavMeshEdge *> NavMeshEdgesList;

	class NavMeshPoint
	{
	public:
		core::Vector2 position;
		NavMeshEdgesList edges;
		NavMeshPoint();
		NavMeshPoint(const core::Vector2 &p);
	};

	class NavMeshTriangle
	{
	public:
		NavMeshTriangle();
		NavMeshPoint* points[3];
		NavMeshEdge* edges[3];
		AStarNode* aStarNode;
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

		virtual void API_CALL clear() OVERRIDE;
		virtual void API_CALL addPolygon(IntPtr vertices2f, uint verticesCount) OVERRIDE;
		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;

	};

	class PathMesh : public llge::IPathMesh
	{
	public:
		void clear();

		virtual int API_CALL getTriagnlesCount() OVERRIDE;
		virtual void API_CALL getTriagnles(IntPtr triangles3f) OVERRIDE;
		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
		std::vector<NavMeshTriangle* > _triangles;
	private:
		core::Vector2 _start;
		core::Vector2 _finish;
	};

	class NavMesh : public llge::INavMesh
	{
	public:
		NavMesh();
		~NavMesh();
		
		void build(NavMeshConfig *config);
		NavMeshPolygon* addPolygon(const std::vector<core::Vector2> &points);
		void removePolygon(NavMeshPolygon *polygon);

		virtual void API_CALL buildNavMesh(llge::INavMeshConfig* config) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
		virtual int API_CALL getTriagnlesCount() OVERRIDE;
		virtual void API_CALL getTriagnles(IntPtr triangles3f) OVERRIDE;
		virtual void API_CALL fillPathMesh(float x0, float y0, float x1, float y1, llge::IPathMesh *mesh) OVERRIDE;
	private:
		ClipperLib::IntPoint toClipper(const core::Vector2 &v) const;
		core::Vector2 toVector(const ClipperLib::IntPoint &v) const;
		core::Vector2 toVector(const p2t::Point &v) const;
		void buildRoot(ClipperLib::PolyNode *root);
		void createRootTriangles(ClipperLib::PolyNode *root);
		void createRootTrianglesWithHolesOffset(ClipperLib::PolyNode *root);
		void clear();
		static NavMeshEdge *findEdge(NavMeshPoint *a, NavMeshPoint *b, bool &isInversed);
		NavMeshEdge *buildEdgeLinks(NavMeshPoint *a, NavMeshPoint *b, NavMeshTriangle *t);
		void buildLinks();
		NavMeshTriangle* findTriangle(const core::Vector2 &position);
		void fillPath(core::Vector2 start, core::Vector2 target, PathMesh* pathMesh);
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