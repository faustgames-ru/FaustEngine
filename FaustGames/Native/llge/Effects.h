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
		static std::string _solidPixelShader;
		static std::string _solidVertexShader;
	public:
		EffectSolid();
		~EffectSolid();
		Effect *getEffect();
		UniformValueMatrix *getProjection();
	};
	
	class Effects
	{
	private:
		static core::Lazy<Effects> _instance;
		static Effects * instanciate();
		static void deinstanciate(Effects *value);

		EffectSolid _solid;

		Effects();
		~Effects();
	public:
		static Effects *instance();
		void create();
		void cleanup();
		EffectSolid * solid();
	};
}


#endif /*EFFECTS_H*/