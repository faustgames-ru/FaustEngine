#include "Uniform.h"
#include "UniformInfo.h"
#include "Errors.h"
#include "Math.h"
#include "Effect.h"

namespace graphics
{
	Uniform::Uniform()
	{
	}
	
	Uniform::~Uniform()
	{
	}

	UniformInfo *Uniform::getInfo()
	{
		return _info;
	}

	void Uniform::init(UniformInfo *value, UniformValue *uniformValue)
	{
		_info = value;
		_valueContainer = uniformValue;
	}

	void Uniform::create(Effect * effect)
	{
		_parameterHandler = glGetUniformLocation(effect->getHandle(), _info->getName());

		Errors::check(Errors::GetUniformLocation);
		_valueContainer->reset();
		_shaderId = effect->getShaderId();
		_shaderMask = effect->getShaderMask();
		if (_info->getType() == UniformType::Sampler2D)
		{
			int samplerIndex = _valueContainer->getSamplerIndex();
			glUniform1i(_parameterHandler, samplerIndex + GraphicsConstants::Samplers2DStart);
			Errors::check(Errors::Uniform1f);
		}
		if (_info->getType() == UniformType::SamplerCube)
		{
			int samplerIndex = _valueContainer->getSamplerIndex();
			glUniform1i(_parameterHandler, samplerIndex + GraphicsConstants::SamplersCubeStart);
			Errors::check(Errors::Uniform1f);
		}
	}

	GLuint Uniform::getHandle()
	{
		return _parameterHandler;
	}	
}