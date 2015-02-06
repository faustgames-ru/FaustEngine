#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics_classes.h"
#include "TextureLoader.h"

namespace graphics
{
	class Texture
	{
	public:
		inline const GLuint getHandle(){ return _handle; }
	protected:
		GLuint _handle;
	};
}

#endif /*TEXTURE_H*/
