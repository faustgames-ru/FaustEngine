#include "EffectsBasic.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "Attributes.h"

namespace graphics
{
	EffectSolidColor EffectsBasic::_solidColor;
	EffectTextureColor EffectsBasic::_textureColor;

	void EffectsBasic::create()
	{
		UniformValues::initSamplers();

		_solidColor.create();
		_textureColor.create();
	}

	void EffectsBasic::cleanup()
	{
		_solidColor.getEffect()->cleanup();
		_textureColor.getEffect()->cleanup();
	}

	EffectSolidColor * EffectsBasic::solidColor()
	{
		return &_solidColor;
	}

	EffectTextureColor * EffectsBasic::textureColor()
	{
		return &_textureColor;
	}
}	
