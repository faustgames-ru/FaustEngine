#include "EffectConstant.h"
#include "Effect.h"
#include "Errors.h"
#include "Color.h"

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

	void EffectConstant::setUint(unsigned int value)
	{
		(*((unsigned int *)_value)) = value;
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

	void EffectConstant::applyColor()
	{
		unsigned int c = getUintValue();
		float red = Color::getRf(c);
		float green = Color::getGf(c);
		float blue = Color::getBf(c);
		float alpha = Color::getAf(c);
		glUniform4f(_parameterHandler, red, green, blue, alpha);
		Errors::check(Errors::Uniform1f);
	}

	void EffectConstant::apply()
	{
		//if (!_valueChanged) return;
		float v = 0;
		switch (_type)
		{
		case UniformType::Float:
			v = getFloatValue();
			glUniform1f(_parameterHandler, v);
			Errors::check(Errors::Uniform1f);
			break;
		case UniformType::Color:
			applyColor();
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