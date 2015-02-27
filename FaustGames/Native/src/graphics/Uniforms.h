#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics_classes.h"
#include "UniformInfo.h"
#include "UniformValueFloat.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"
#include "UniformValueCubemap.h"

namespace graphics
{
	class Uniforms
	{
	public:
		static UniformInfo *projection();
		static UniformInfo *texture();
		static UniformInfo *lightmap();
		static UniformInfo *environment();
	private:
		static UniformInfo _projection;		
		static UniformInfo _texture;
		static UniformInfo _lightmap;
		static UniformInfo _environment;
	};

	class UniformValues
	{
	public:
		static inline UniformValueMatrix *projection(){ return &_projection; }
		static inline UniformValueTexture *texture(){ return &_texture; }
		static inline UniformValueTexture *lightmap(){ return &_lightmap; }
		static inline UniformValueCubemap *environment(){ return &_environment; }
	private:
		static UniformValueMatrix _projection;
		static UniformValueTexture _texture;
		static UniformValueTexture _lightmap;
		static UniformValueCubemap _environment;
	};
}

#endif /*UNIFORM_INFO_H*/