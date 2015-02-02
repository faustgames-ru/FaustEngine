#ifndef EFFECT_SOLID_H
#define EFFECT_SOLID_H

#include "graphics.h"
#include "Effect.h"
#include "EffectBase.h"
#include "Lazy.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"

namespace graphics
{
	class EffectSolid : public EffectBase
	{
	public:
		EffectSolid();
		~EffectSolid();
		void create();
	protected:
	private:
		static std::string _pixelShader;
		static std::string _vertexShader;
	};
}

#endif /*EFFECT_SOLID_H*/
