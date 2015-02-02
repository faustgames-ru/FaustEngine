#include "Effects.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "ObjectsPool.h"

namespace graphics
{
	EffectSolid Effects::_solid;
	EffectSolidColor Effects::_solidColor;
	EffectTextureColor Effects::_textureColor;


	void Effects::create()
	{
		_solid.create();
		_solidColor.create();
		_textureColor.create();
	}

	void Effects::cleanup()
	{
		_solid.getEffect()->cleanup();
		_solidColor.getEffect()->cleanup();
		_textureColor.getEffect()->cleanup();
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
}