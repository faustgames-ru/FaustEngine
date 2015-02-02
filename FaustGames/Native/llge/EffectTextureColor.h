#ifndef EFFECT_TEXTURE_COLOR_H
#define EFFECT_TEXTURE_COLOR_H

#include "graphics.h"
#include "Effect.h"
#include "EffectBase.h"
#include "Lazy.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectTextureColor : public EffectBase
	{
	public:
		EffectTextureColor();
		~EffectTextureColor();
		void create();
	protected:
	private:
		static std::string _pixelShader;
		static std::string _vertexShader;
	};
}

#endif /*EFFECT_TEXTURE_COLOR_H*/
