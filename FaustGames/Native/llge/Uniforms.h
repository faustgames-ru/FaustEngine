#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics.h"

namespace graphics
{
	class Uniforms
	{
	private:
		static Uniforms *_instance;
		UniformInfo _projection;
		UniformInfo _texture;
		UniformInfo _shadowmap;
		Uniforms();
	public:
		static Uniforms *instance();

		UniformInfo *projection();
		UniformInfo *texture();
		UniformInfo *shadowmap();
	};
}

#endif /*UNIFORM_INFO_H*/