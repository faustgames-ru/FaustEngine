#include "EffectWater.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "..\..\shaders\water_frag.h"
#include "..\..\shaders\water_vert.h"

namespace graphics
{
	EffectWater::EffectWater()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::environment(), UniformValues::environment());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::normal());
	}

	EffectWater::~EffectWater()
	{
	}

	void EffectWater::create()
	{
		_effect.create((char *)shader_water_vert, shader_water_vert_size, (char *)shader_water_frag, shader_water_frag_size);
	}
}