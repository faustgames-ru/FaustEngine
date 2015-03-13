#include "Effects.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "Attributes.h"

namespace graphics
{
	EffectSolid Effects::_solid;
	EffectSolidColor Effects::_solidColor;
	EffectTextureColor Effects::_textureColor;
	EffectTextureLightMapColor Effects::_textureLightmapColor;
	EffectWater Effects::_water;

	void Effects::create()
	{
		_solid.create();
		_solidColor.create();
		_textureColor.create();
		_textureLightmapColor.create();
		_water.create();
	}

	void Effects::cleanup()
	{
		_solid.getEffect()->cleanup();
		_solidColor.getEffect()->cleanup();
		_textureColor.getEffect()->cleanup();
		_textureLightmapColor.getEffect()->cleanup();
		_water.getEffect()->cleanup();
	}

	EffectSolid * Effects::solid()
	{
		return &_solid;
	}

	EffectSolidColor * Effects::solidColor()
	{
		return &_solidColor;
	}

	EffectTextureColor * Effects::textureColor()
	{
		return &_textureColor;
	}

	EffectTextureLightMapColor * Effects::textureLightmapColor()
	{
		return &_textureLightmapColor;
	}

	EffectWater * Effects::water()
	{
		return &_water;
	}


}