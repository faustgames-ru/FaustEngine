#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphics.h"
#include "Effect.h"
#include "Lazy.h"
#include "UniformInfo.h"
#include "Uniform.h"

namespace graphics
{
	class EffectSolid
	{
		Effect _effect;
		UniformValueMatrix _projection;
		static std::string _pixelShader;
		static std::string _vertexShader;
	public:
		EffectSolid();
		~EffectSolid();
		Effect *getEffect();
		UniformValueMatrix *getProjection();
	};
	
	class EffectSolidColor
	{
		Effect _effect;
		UniformValueMatrix _projection;
		static std::string _pixelShader;
		static std::string _vertexShader;
	public:
		EffectSolidColor();
		~EffectSolidColor();
		Effect *getEffect();
		UniformValueMatrix *getProjection();
	};

	class EffectTextureColor
	{
		Effect _effect;
		UniformValueMatrix _projection;
		UniformValueTexture _texture;
		static std::string _pixelShader;
		static std::string _vertexShader;
	public:
		EffectTextureColor();
		~EffectTextureColor();
		Effect *getEffect();
		UniformValueMatrix *getProjection();
		UniformValueTexture *getTexture();
	};

	class Effects
	{
	private:
		static core::Lazy<Effects> _instance;
		static Effects * instanciate();
		static void deinstanciate(Effects *value);

		EffectSolid _solid;
		EffectSolidColor _solidColor;
		EffectTextureColor _textureColor;

		Effects();
		~Effects();
	public:
		static Effects *instance();
		void create();
		void cleanup();
		EffectSolid * solid();
		EffectSolidColor * solidColor();
		EffectTextureColor * textureColor();
	};
}


#endif /*EFFECTS_H*/