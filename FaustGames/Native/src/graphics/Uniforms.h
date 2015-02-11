#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics_classes.h"
#include "UniformInfo.h"
#include "UniformValueFloat.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class Uniforms
	{
	public:
		static UniformInfo *projection();
		static UniformInfo *texture();
		static UniformInfo *lightmap();
	private:
		static UniformInfo _projection;		
		static UniformInfo _texture;
		static UniformInfo _lightmap;
	};

	class UniformValues
	{
	public:
		static inline UniformValueMatrix *projection(){ return &_projection; }
		static inline UniformValueTexture *texture(){ return &_texture; }
		static inline UniformValueTexture *lightmap(){ return &_lightmap; }
	private:
		static UniformValueMatrix _projection;
		static UniformValueTexture _texture;
		static UniformValueTexture _lightmap;
	};
}

#endif /*UNIFORM_INFO_H*/