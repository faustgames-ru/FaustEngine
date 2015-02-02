#ifndef EFFECT_SOLID_COLOR_H
#define EFFECT_SOLID_COLOR_H

#include "graphics.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"

namespace graphics
{
	class EffectSolidColor : public EffectBase
	{
	public:
		EffectSolidColor();
		~EffectSolidColor();
		void create();
	protected:
	private:
		static std::string _pixelShader;
		static std::string _vertexShader;
	};
}

#endif /*EFFECT_SOLID_COLOR_H*/
