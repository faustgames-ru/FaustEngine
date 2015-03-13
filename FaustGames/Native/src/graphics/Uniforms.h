#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics_classes.h"
#include "UniformInfo.h"
#include "UniformValueFloat.h"
#include "UniformValueVector3.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"
#include "UniformValueCubemap.h"

namespace graphics
{
	class Uniforms
	{
	public:
		static UniformInfo *cameraPosition();
		static UniformInfo *projection();
		static UniformInfo *texture();
		static UniformInfo *lightmap();
		static UniformInfo *environment();
		static UniformInfo *normalmap();
		static UniformInfo *time();
	private:
		static UniformInfo _cameraPosition;
		static UniformInfo _projection;
		static UniformInfo _texture;
		static UniformInfo _lightmap;
		static UniformInfo _environment;
		static UniformInfo _normalmap;
		static UniformInfo _time;
	};

	class UniformValues
	{
	public:
		static inline UniformValueVector3 * cameraPosition(){ return &_cameraPosition; }
		static inline UniformValueMatrix *projection(){ return &_projection; }
		static inline UniformValueTexture *texture(){ return &_texture; }
		static inline UniformValueTexture *lightmap(){ return &_lightmap; }
		static inline UniformValueTexture *environment(){ return &_environment; }
		static inline UniformValueTexture *normalmap(){ return &_normalmap; }
		static inline UniformValueFloat *time(){ return &_time; }
	private:
		static UniformValueVector3 _cameraPosition;
		static UniformValueMatrix _projection;
		static UniformValueTexture _texture;
		static UniformValueTexture _lightmap;
		static UniformValueTexture _environment;
		static UniformValueTexture _normalmap;
		static UniformValueFloat _time;
	};
}

#endif /*UNIFORM_INFO_H*/