#include "DynamicVertexLightMap.h"

namespace drawing
{
	uint EmptyLightMap::applyLight(float x, float y, uint color, byte colorScale, bool additive)
	{
		return graphics::Color::premul(color, colorScale, additive);
	}

	int DynamicVertexLightMap::getTileX(float x, int add) const
	{
		return core::Math::clampi(static_cast<int>(add + ((x - _x) / _w)*(lightsUsageTilesSize - 1)), 0, lightsUsageTilesSize - 1);
	}

	int DynamicVertexLightMap::getTileY(float y, int add) const
	{
		return core::Math::clampi(static_cast<int>(add + ((y - _y) / _h)*(lightsUsageTilesSize - 1)), 0, lightsUsageTilesSize - 1);
	}

	int DynamicVertexLightMap::getTileIndex(float x, float y) const
	{
		int ix = getTileX(x, 0);
		int iy = getTileY(y, 0);
		return iy*lightsUsageTilesSize + ix;
	}

	int DynamicVertexLightMap::getTileIndex(int x, int y)
	{
		return y*lightsUsageTilesSize + x;
	}

	DynamicVertexLightMap::DynamicVertexLightMap(): _x(0), _y(0), _w(1), _h(1), _ambientR(0), _ambientG(0), _ambientB(0)
	{
		// precalc lighting
		int adr = 0;
		for (int y = 0; y < lightPrecalcSize; y++)
		{
			float yu = static_cast<float>(y) / static_cast<float>(lightPrecalcSize - 1);			
			float fy = core::Math::lerp(-1.0f, 1.0f, yu);
			for (int x = 0; x < lightPrecalcSize; x++)
			{
				float xu = static_cast<float>(x) / static_cast<float>(lightPrecalcSize - 1);
				float fx = core::Math::lerp(-1.0f, 1.0f, xu);
				float l = core::Math::saturate(core::Vector2(fx, fy).length());
				_lightPrecalc[adr] = static_cast<byte>((1.0f - l) * 255);
				adr++;
			}
		}
		clear();
		build(0, 0, 1, 1, 0xffffffff);
	}

	void DynamicVertexLightMap::clear()
	{
		_lights.count = 0;
		int tilesCount = lightsUsageTilesSize*lightsUsageTilesSize;
		for (int i = 0; i < tilesCount; i++)
		{
			_lightsUsageTiles[i].count = 0;
		}
	}

	void DynamicVertexLightMap::addLight(float x, float y, float r, uint color, float intensity)
	{
		if (!_lights.canAdd()) return;
		DynamicLight light;
		light.x = x;
		light.y = y;
		light.r = r;
		light.invD = static_cast<float>(lightPrecalcSize - 1) / (r*2.0f);
		light.cr = graphics::Color::getRf(color)*intensity;
		light.cg = graphics::Color::getGf(color)*intensity;
		light.cb = graphics::Color::getBf(color)*intensity;
		_lights.add(light);
	}

	void DynamicVertexLightMap::build(float x, float y, float w, float h, uint ambient)
	{
		_ambientR = graphics::Color::getRf(ambient);
		_ambientG = graphics::Color::getGf(ambient);
		_ambientB = graphics::Color::getBf(ambient);
		_x = x;
		_y = y;
		_w = w;
		_h = h;
		// fill _lightsUsageTiles map
		for (int i = 0; i < _lights.count; i++)
		{
			DynamicLight* light = _lights.data + i;
			int minX = getTileX(light->x - light->r, -1);
			int minY = getTileY(light->y - light->r, -1);
			int maxX = getTileX(light->x + light->r, 1);
			int maxY = getTileY(light->y + light->r, 1);
			for (int iy = minY; iy <= maxY; iy++)
			{
				int adr = getTileIndex(minX, iy);
				for (int ix = minX; ix <= maxX; ix++)
				{
					_lightsUsageTiles[adr].addWithRangeCheck(light);
					adr++;
				}
			}
		}
	}

	uint DynamicVertexLightMap::applyLight(float x, float y, uint color, byte colorScale, bool additive)
	{
		core::StaticArray<DynamicLight*, lightsUsageTilesDepth>* tileLights = _lightsUsageTiles + getTileIndex(x, y);
		float sr = graphics::Color::getRf(color);
		float sg = graphics::Color::getGf(color);
		float sb = graphics::Color::getBf(color);
		float sa = graphics::Color::getAf(color);
		float r = _ambientR;
		float g = _ambientG;
		float b = _ambientB;
		float s = static_cast<float>(colorScale) / 255.0f;
		if (tileLights->count > 2)
		{
			tileLights->count = tileLights->count;
		}
		for (int i = 0; i < tileLights->count; i++)
		{
			DynamicLight* light = tileLights->data[i];

			// todo: applyLightDistance;

			float minX = light->x - light->r;
			float minY = light->y - light->r;

			float invD = light->invD;
			int ix = static_cast<int>((x - minX) * invD);
			int iy = static_cast<int>((y - minY) * invD);

			if (ix < 0) continue;
			if (ix >= lightPrecalcSize) continue;
			if (iy < 0) continue;
			if (iy >= lightPrecalcSize) continue;

			float lightingLevel = static_cast<float>(_lightPrecalc[iy*lightPrecalcSize + ix]) / 255.0f;
			r += light->cr*lightingLevel;
			g += light->cg*lightingLevel;
			b += light->cb*lightingLevel;
		}

		float a = sa;
		if (additive)
		{
			a = 0.0f;
		}

		return graphics::Color::fromRgba(sr*r*sa*s, sg*g*sa*s, sb*b*sa*s, a);
	}
}
