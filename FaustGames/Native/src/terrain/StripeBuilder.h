#ifndef TERRAIN_STRIPE_BUILDER_H
#define TERRAIN_STRIPE_BUILDER_H

#include "terrain_classes.h"

namespace terrain
{
	struct StripePoint
	{
		float x;
		float y;
		float tension;
		float width;

		StripePoint offset(float dx, float dy);
	};

	struct StripeListPoint
	{
		StripePoint point;
		core::Vector2 n;
		core::Vector2 d;
		core::Vector2 prevD;
		core::Vector2 nextD;
		float scale;
		StripeListPoint *next;
		StripeListPoint *prev;
	};

	struct StripeType
	{
		enum e
		{
			None = 0x0,
			Tile = 0x1,
			TileCap = 0x2,
			TileAngular = 0x4,
			All = Tile | TileCap| TileAngular,
		};
	};

	struct StripeConfig
	{
		StripeType::e AllowedTiles;
		float DetailX;
		float DetailY;
		StripeConfig();
	};

	struct StripeTile
	{
		StripeType type;
		StripeTile *nexTile;

	};

	class StripeBuilder: public llge::ITerrainStripeBuilder
	{
	public:
		StripeConfig config;
		void build(const StripePoint *vertices, int count, bool closed);
		
		virtual void API_CALL buildStripe(IntPtr terrainStripePoints, int count, bool closed) OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
		virtual int API_CALL getDebugRenderVerticesCount() OVERRIDE;
		virtual void API_CALL getDebugRenderVertices(IntPtr vertices2f) OVERRIDE;

	private:
		std::vector<core::Vector2> _dedugRender;
	};
}

#endif TERRAIN_STRIPE_BUILDER_H
