#ifndef GRPAHICS_PIPELINES_UNIFORMS_CONFIG_H
#define GRPAHICS_PIPELINES_UNIFORMS_CONFIG_H

#include "../graphics_classes.h"
#include "../graphics.h"

namespace graphics
{
	class IUniformConfig : IBaseObject
	{
	public:
		virtual void apply() = 0;
	};


	struct UniformConfigNone : IUniformConfig
	{
		virtual void apply() OVERRIDE;
	};
	
	struct UniformConfigTexture: IUniformConfig
	{
		uint testureId;
		virtual void apply() OVERRIDE;
	};
}

#endif /*GRPAHICS_PIPELINES_UNIFORMS_CONFIG_H*/