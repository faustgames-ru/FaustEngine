#ifndef DYNAMIC_VERTEX_LIGHT_MAP_H
#define DYNAMIC_VERTEX_LIGHT_MAP_H

#include "drawing_classes.h"

namespace drawing
{
	class DynamicLight
	{
	public:
		float x;
		float y;
		float r;
		float invD;
		float cr;
		float cg;
		float cb;
	};

	class ILightMap: IBaseObject
	{
	public:
		virtual uint applyLight(float x, float y, uint color, byte colorScale, bool additive) = 0;
	};

	class EmptyLightMap : public ILightMap
	{
	public:	
		virtual uint applyLight(float x, float y, uint color, byte colorScale, bool additive) override;
	};

	class DynamicVertexLightMap: public ILightMap
	{
	private:
		static const int lightsUsageTilesSize = 32;
		static const int lightsUsageTilesDepth = 32;
		static const int lightPrecalcSize = 32;
		static const int lightsLimit = 1024;

		byte _lightPrecalc[lightPrecalcSize*lightPrecalcSize];
		core::StaticArray<DynamicLight, lightsLimit> _lights;
		core::StaticArray<DynamicLight*, lightsUsageTilesDepth> _lightsUsageTiles[lightsUsageTilesSize*lightsUsageTilesSize];
		float _x;
		float _y;
		float _w;
		float _h;
		float _ambientR;
		float _ambientG;
		float _ambientB;
		int getTileX(float x, int add) const;
		int getTileY(float y, int add) const;
		int getTileIndex(float x, float y) const;
		static int getTileIndex(int x, int y);
	public:
		DynamicVertexLightMap();
		void clear();
		void addLight(float x, float y, float r, uint color, float intensity);
		void build(float x, float y, float w, float h, uint ambient);
		virtual uint applyLight(float x, float y, uint color, byte colorScale, bool additive) override;
	};
}

#endif // DYNAMIC_VERTEX_LIGHT_MAP_H