#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics.h"
#include "core.h"
#include "Lazy.h"
#include "UniformInfo.h"

namespace graphics
{
	class Uniforms
	{
	private:
		static core::Lazy<Uniforms> _instance;
		static Uniforms *instanciate();
		static void deinstanciate(Uniforms *value);
		UniformInfo _projection;
		UniformInfo _texture;
		UniformInfo _shadowmap;
		Uniforms();
		~Uniforms();
	public:
		static Uniforms *instance();

		UniformInfo *projection();
		UniformInfo *texture();
		UniformInfo *shadowmap();
	};
}

#endif /*UNIFORM_INFO_H*/