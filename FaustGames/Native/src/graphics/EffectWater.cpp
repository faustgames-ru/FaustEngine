#include "EffectWater.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "..\..\shaders\water_frag.h"
#include "..\..\shaders\water_vert.h"

namespace graphics
{

	EffectWater::EffectWater()
	{
		_effect.addConstant("textureVelocity", UniformType::Float);
		_effect.addConstant("wavesVelocity", UniformType::Float);
		_effect.addConstant("wavesScale", UniformType::Float);
		_effect.addConstant("normalScaleZ", UniformType::Float);
		_effect.addConstant("envOffsetY", UniformType::Float);
		_effect.addConstant("envScaleY", UniformType::Float);

		_effect.addUniform(Uniforms::cameraPosition(), UniformValues::cameraPosition());
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::environment(), UniformValues::environment());
		_effect.addUniform(Uniforms::normalmap(), UniformValues::normalmap());
		_effect.addUniform(Uniforms::time(), UniformValues::time());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectWater::~EffectWater()
	{
	}

	void EffectWater::create()
	{
		_effect.create((char *)shader_water_vert, shader_water_vert_size, (char *)shader_water_frag, shader_water_frag_size);
	}
}