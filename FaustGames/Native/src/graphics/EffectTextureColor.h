#ifndef EFFECT_TEXTURE_COLOR_H
#define EFFECT_TEXTURE_COLOR_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectTexturePackedColor : public EffectBase
	{
	public:
		EffectTexturePackedColor();
		~EffectTexturePackedColor();
		void create();
		virtual void configApply(const void *config) override;
		virtual EffectBase* GetEtc1A8Effect() override  { return this; } // todo: write correct shader for etc1a8
	protected:
	private:
	};

	class EffectTextureColor : public EffectBase
	{
	public:
		EffectTextureColor();
		~EffectTextureColor();
		void create();
		virtual void configApply(const void *config) OVERRIDE;
		virtual EffectBase* GetEtc1A8Effect() override;

	protected:
	private:
	};

	class EffectTextureColorEtc1A8 : public EffectBase
	{
	public:
		EffectTextureColorEtc1A8();
		~EffectTextureColorEtc1A8();
		void create();
		virtual void configApply(const void *config) override;
		virtual EffectBase* GetEtc1A8Effect() override  { return this; }
	protected:
	private:
	};

	class EffectTextureColorFog : public EffectBase
	{
	public:
		EffectTextureColorFog();
		~EffectTextureColorFog();
		void create();
		virtual void configApply(const void *config) override;
		virtual EffectBase* GetEtc1A8Effect() override;
	protected:
	private:
	};

	class EffectTextureColorFogEtc1A8 : public EffectBase
	{
	public:
		EffectTextureColorFogEtc1A8();
		~EffectTextureColorFogEtc1A8();
		void create();
		virtual void configApply(const void *config) override;
		virtual EffectBase* GetEtc1A8Effect() override { return this; };
	protected:
	private:
	};

	class EffectTextureRgbTransform : public EffectBase
	{
	public:
		EffectTextureRgbTransform();
		~EffectTextureRgbTransform();
		void create();
		virtual void configApply(const void *config) override;
		virtual EffectBase* GetEtc1A8Effect() override;
	protected:
	private:
	};

	class EffectTextureRgbTransformEtc1A8 : public EffectBase
	{
	public:
		EffectTextureRgbTransformEtc1A8();
		~EffectTextureRgbTransformEtc1A8();
		void create();
		virtual void configApply(const void *config) override;
		virtual EffectBase* GetEtc1A8Effect() override { return this; };
	protected:
	private:
	};

}

#endif /*EFFECT_TEXTURE_COLOR_H*/
