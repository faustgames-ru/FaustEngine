#include "EffectConstant.h"
#include "Effect.h"
#include "Errors.h"

namespace graphics
{
	void EffectConstant::init(const char *name, UniformType::e type)
	{
		_name = name;
		_type = type;
		_valueChanged = true;
		for (int i = 0; i < bufferSize; i++)
			_value[i] = 0;
	}
	
	void EffectConstant::setFloat(float value)
	{
		(*((float *)_value)) = value;
		setChanged();
	}

	void EffectConstant::create(Effect *effect)
	{
		_owner = effect;
		_parameterHandler = glGetUniformLocation(effect->getHandle(), _name.c_str());
		Errors::check(Errors::GetUniformLocation);
	}
	
	void EffectConstant::setChanged()
	{
		_valueChanged = true;
		_owner->setConstantsChanged();
	}


	void EffectConstant::apply()
	{
		if (!_valueChanged) return;
		switch (_type)
		{
		case UniformType::Float:
			glUniform1f(_parameterHandler, getFloatValue());
			Errors::check(Errors::Uniform1f);
			break;
		default:
			/// not supported constant type
			break;
		}
		_valueChanged = false;
	}

	bool EffectConstant::nameEqualsTo(const char *name)
	{
		return _name == name;
	}


}