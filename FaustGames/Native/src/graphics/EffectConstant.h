#ifndef EFFECT_CONSTANT_H
#define EFFECT_CONSTANT_H

#include "graphics_classes.h"

namespace graphics
{
	class EffectConstant
	{
	public:
		EffectConstant(){}
		void init(const char *name, UniformType::e type);
		void setFloat(float value);
		void create(Effect *effect);
		void apply();
		bool nameEqualsTo(const char *name);
	private:
		inline float getFloatValue(){ return *((float *)_value); }
		void setChanged();
		std::string _name;
		UniformType::e _type;
		static const int bufferSize = 16;
		char _value[bufferSize];
		bool _valueChanged;
		GLuint _parameterHandler;
		Effect *_owner;
	};
}

#endif