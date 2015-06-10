#ifndef EFFECT_POST_PROCESS_H
#define EFFECT_POST_PROCESS_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"

namespace graphics
{	
	class EffectPostProcessToneMapping : public EffectBase
	{
	public:
		EffectPostProcessToneMapping();
		~EffectPostProcessToneMapping();
		void create();
	protected:
	private:
	};

	class EffectPostProcessBloomFilter : public EffectBase
	{
	public:
		EffectPostProcessBloomFilter();
		~EffectPostProcessBloomFilter();
		void create();
	protected:
	private:
	};	

	class EffectPostProcessVBlurFilter : public EffectBase
	{
	public:
		EffectPostProcessVBlurFilter();
		~EffectPostProcessVBlurFilter();
		void create();
	protected:
	private:
	};


	class EffectPostProcessHBlurFilter : public EffectBase
	{
	public:
		EffectPostProcessHBlurFilter();
		~EffectPostProcessHBlurFilter();
		void create();
	protected:
	private:
	};

	class EffectPostProcessBloomAddFilter : public EffectBase
	{
	public:
		EffectPostProcessBloomAddFilter();
		~EffectPostProcessBloomAddFilter();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_POST_PROCESS_H*/
