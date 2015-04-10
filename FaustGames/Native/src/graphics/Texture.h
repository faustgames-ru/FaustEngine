#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics_classes.h"
#include "TextureLoader.h"

namespace graphics
{
	class Texture : public llge::ITexture
	{
	public:
		inline const GLuint getHandle(){ return _handle; }
		virtual uint API_CALL getId() { return getHandle(); }
		inline void setHandle(GLuint value) { _handle = value; }
		virtual void API_CALL dispose() { delete this; }
	protected:
		GLuint _handle;
	};
}

#endif /*TEXTURE_H*/
