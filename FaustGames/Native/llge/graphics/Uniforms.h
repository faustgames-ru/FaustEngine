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
		static UniformInfo *shadowmap();
	private:
		static UniformInfo _projection;		
		static UniformInfo _texture;
		static UniformInfo _shadowmap;
	};

	class UniformValues
	{
	public:
		static inline UniformValueMatrix *projection(){ return &_projection; }
		static inline UniformValueTexture *texture(){ return &_texture; }
		static inline UniformValueTexture *shadowmap(){ return &_shadowmap; }
	private:
		static UniformValueMatrix _projection;
		static UniformValueTexture _texture;
		static UniformValueTexture _shadowmap;
	};
}

#endif /*UNIFORM_INFO_H*/