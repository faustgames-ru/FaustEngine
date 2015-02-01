#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics.h"
#include "UniformInfo.h"

namespace graphics
{
	class Uniforms
	{
	private:		
		static UniformInfo _projection;		
		static UniformInfo _texture;
		static UniformInfo _shadowmap;
	public:
		static UniformInfo *projection();
		static UniformInfo *texture();
		static UniformInfo *shadowmap();
	};
}

#endif /*UNIFORM_INFO_H*/