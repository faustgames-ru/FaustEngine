#ifndef TERRAIN_STRIPE_BUILDER_H
#define TERRAIN_STRIPE_BUILDER_H

#include "terrain_classes.h"
#include "../core/Spline.h"
#include "../geometry/TerrainClipper.h"

namespace terrain
{
	class Stripe;

	struct StripePoint
	{
		float x;
		float y;
		float wt;
		float wb;
		float t;

		StripePoint offset(float dx, float dy);
		StripePoint offset(core::Vector2 d);
		StripePoint copy(core::Vector2 p);
		core::Vector2 position();
	};

	struct StripeListPoint
	{
		StripePoint point;
		StripePoint pt;
		StripePoint pb;
		core::Vector2 n;
		core::Vector2 d;
		core::Vector2 prevD;
		core::Vector2 nextD;
		float scale;
		float tl;
		float tr;
		float bl;
		float br;
		bool large;
		StripeListPoint *next;
		StripeListPoint *prev;
		int tilesCount;
	};

	struct ListPoint
	{
		core::Vector2 p;
		ListPoint *next;
		ListPoint *prev;
	};
	
	struct StripeType
	{
		enum e
		{
			None = 0x0,
			Tile = 0x1,
			TileCap = 0x2,
			TileAngular = 0x4,
			Platform = Tile | TileCap,
			Terrain = Tile | TileAngular,
			All = Tile | TileCap | TileAngular,
		};
	};

	struct TileStripeType
	{
		enum e
		{
			None = 0x0,
			CapLeft = 0x1,
			CapRight = 0x2,
			TileTop = 0x3,
			TileBottom = 0x4,
			TileLeft = 0x5,
			TileRight = 0x6,
			AngularLeftToTop = 0x7,
			AngularTopToRight = 0x8,
			AngularRightToBottom = 0x9,
			AngularBottomToLeft = 0xA,
			AngularLeftToBottom = 0xB,
			AngularBottomToRight = 0xC,
			AngularRightToTop = 0xD,
			AngularTopToLeft = 0xE,
		};
	};
		
		
	struct StripeWidth
	{
		float out;
		float in;
		float width;
		StripeWidth();
		StripeWidth(float out, float in, float width);
	};

	struct StripeConfig : public llge::ITerrainConfig
	{
		StripeType::e AllowedTiles;
		llge::TerrainStripePhysicsType PhysicsMode;
		int DetailX;
		bool flipReversed;
		StripeWidth left;
		StripeWidth right;
		StripeWidth top;
		StripeWidth bottom;

		StripeConfig();
		StripeWidth getWidth(core::Direction2::e d);
		TileStripeType::e getType(core::Direction2::e d);
		
		virtual void API_CALL set(llge::TerrainStripeConfigType type, float inWidth, float outWidth, float width) OVERRIDE;
		virtual void API_CALL setPhysicsMode(llge::TerrainStripePhysicsType mode) OVERRIDE;
		virtual void API_CALL setFlipReversed(bool value) OVERRIDE;
		virtual void API_CALL setTilesConfig(uint value) OVERRIDE;
	};

	struct StripeTile
	{
		TileStripeType::e type;
		core::MeshIndexer indexer;
	};
	
	struct StripeEdge
	{
		core::Vector2 p[4];
	};

	struct SegmentConfig
	{
		TileStripeType::e type;
		core::Vector2 centerPosition;
		core::Vector2 startPosition;

		core::Vector2 centerTexture;
		core::Vector2 startTexture;
		core::Vector2 finishTexture;

		core::MacroSpline2* spline;
		float statLen;
		float finishLen;
	};

	class StripeBuilder: public llge::ITerrainStripeBuilder
	{
	public:
		StripeConfig config;
		void build(const StripePoint *vertices, float tension, int count, bool closed);
		virtual llge::ITerrainConfig* API_CALL getConfig() OVERRIDE;
		virtual void API_CALL buildStripe(IntPtr terrainStripePoints, float tension, int count, bool closed) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
		virtual int API_CALL getDebugRenderVerticesCount() OVERRIDE;
		virtual void API_CALL getDebugRenderVertices(IntPtr vertices2f) OVERRIDE;
		virtual llge::IMeshesResult* API_CALL getResult() OVERRIDE;
		virtual int API_CALL getEdgesCount() OVERRIDE;
		virtual void API_CALL getEdge(int i, IntPtr vertices2f) OVERRIDE;

	private:
		void initQuad1Indices();
		void initQuadIndices(int count);
		void initFanIndices(int count);
		void initFan2Indices(int count);
		core::Vector2 buildSegment(TileStripeType::e type, core::Spline2& spline, float l0, float l1, core::Vector2 start, core::Vector2 center, bool large, bool inverse);
		core::Vector2 buildSegment(const SegmentConfig &seg, bool inverse);
		StripeListPoint * createStripeList(const StripePoint* vertices, int count, bool closed);
		void addQuad(TileStripeType::e type, core::MacroSpline2* t, core::MacroSpline2* b, float tl0, float bl0, float tl1, float bl1, core::Vector2 &startT, core::Vector2 &startB);
		void addQuad(TileStripeType::e type, core::Spline2& t, core::Spline2& b, float tl0, float bl0, float tl1, float bl1, core::Vector2 &startT, core::Vector2 &startB);
		void addQuad(TileStripeType::e type, core::Vector2 lb, core::Vector2 lt, core::Vector2 rt, core::Vector2 rb);
		std::vector<core::Vector2> _dedugRender;
		std::vector<StripeTile> _tiles;
		std::vector<core::MeshVertex> _vertices;
		std::vector<ushort> _indices;
		uint _quad1IndicesStart;
		uint _quad1IndicesCount;
		uint _quadIndicesStart;
		uint _quadIndicesCount;
		uint _fanIndicesStart;
		uint _fanIndicesCount;
		uint _inverseFanIndicesStart;
		uint _inverseFanIndicesCount;
		uint _fan2IndicesStart;
		uint _fan2IndicesCount;
		geometry::MeshesResult _result;
		std::vector<core::Vector2> _polyline;
		std::vector<StripeEdge> _edges;
	};
}

#endif /*TERRAIN_STRIPE_BUILDER_H*/
