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
	class EffectPostProcessBase : public EffectBase 
	{
		virtual EffectBase* GetEtc1A8Effect() { return this; }
	};
	
	class EffectPostProcessToneMapping : public EffectPostProcessBase
	{
	public:
		EffectPostProcessToneMapping();
		~EffectPostProcessToneMapping();
		void create();
	protected:
	private:
	};

	class EffectPostProcessBloomFilter : public EffectPostProcessBase
	{
	public:
		EffectPostProcessBloomFilter();
		~EffectPostProcessBloomFilter();
		void create();
	protected:
	private:
	};	

	class EffectPostProcessEmpty : public EffectPostProcessBase
	{
	public:
		EffectPostProcessEmpty();
		~EffectPostProcessEmpty();
		void create();
	protected:
	private:
	};


	class EffectPostProcessEmptyColor : public EffectPostProcessBase
	{
	public:
		EffectPostProcessEmptyColor();
		~EffectPostProcessEmptyColor();
		void create();
	protected:
	private:
	};

	class EffectPostProcessVBlurFilter : public EffectPostProcessBase
	{
	public:
		EffectPostProcessVBlurFilter();
		~EffectPostProcessVBlurFilter();
		void create();
	protected:
	private:
	};


	class EffectPostProcessHBlurFilter : public EffectPostProcessBase
	{
	public:
		EffectPostProcessHBlurFilter();
		~EffectPostProcessHBlurFilter();
		void create();
	protected:
	private:
	};

	class EffectPostProcessBloomAddFilter : public EffectPostProcessBase
	{
	public:
		EffectPostProcessBloomAddFilter();
		~EffectPostProcessBloomAddFilter();
		void create();
	protected:
	private:
	};

	class EffectPostProcessBloomDepthAddFilter : public EffectPostProcessBase
	{
	public:
		EffectPostProcessBloomDepthAddFilter();
		~EffectPostProcessBloomDepthAddFilter();
		void create();
	protected:
	private:
	};

	class EffectPostProcessMix : public EffectPostProcessBase
	{
	public:
		EffectPostProcessMix();
		~EffectPostProcessMix();
		void create();
	protected:
	private:
	};

	class EffectPostProcessBlurPass : public EffectPostProcessBase
	{
	public:
		EffectPostProcessBlurPass();
		~EffectPostProcessBlurPass();
		void create();
	protected:
	private:
	};

	class EffectPostProcessRgbTransform : public EffectPostProcessBase
	{
	public:
		EffectPostProcessRgbTransform();
		~EffectPostProcessRgbTransform();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_POST_PROCESS_H*/
