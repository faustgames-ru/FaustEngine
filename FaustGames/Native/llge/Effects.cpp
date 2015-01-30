#include "Effects.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "ObjectsPool.h"

namespace graphics
{
	Effects::Effects()
	{
	}

	void Effects::create()
	{
		_solid.getEffect()->create();
	}

	void Effects::cleanup()
	{
		_solid.getEffect()->cleanup();
	}
	
	EffectSolid * Effects::solid()
	{
		return &_solid;
	}
	
	Effects::~Effects()
	{
	}

	Effects * Effects::instanciate()
	{
		return new Effects();
	}
	
	void Effects::deinstanciate(Effects *value)
	{
		delete value;
	}

	core::Lazy<Effects> Effects::_instance(Effects::instanciate, Effects::deinstanciate);


	Effects * Effects::instance()
	{
		return _instance.value();
	}

	EffectSolid::EffectSolid()
	{
		_effect.setCode(_solidVertexShader.c_str(), _solidPixelShader.c_str());
		_effect.addUniform(Uniforms::instance()->projection(), &_projection);
	}
	
	EffectSolid::~EffectSolid()
	{
	}

	Effect *EffectSolid::getEffect()
	{
		return &_effect;
	}
	UniformValueMatrix *EffectSolid::getProjection()
	{
		return &_projection;
	}


	std::string EffectSolid::_solidPixelShader(
"\
void main() \
{\
gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\
}\
");

	std::string EffectSolid::_solidVertexShader(
"\
attribute vec3 position;\
uniform mat4 projection;\
\
void main()\
{\
gl_Position = projection *  vec4(position, 1.0);\
}\
");

}