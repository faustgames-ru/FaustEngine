#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics_classes.h"
#include "TextureLoader.h"

namespace graphics
{
	class Texture : public llge::ITexture
	{
	public:
		inline const GLuint getHandle()
		{ 
			if (!_handle)
				return _handleDefault;
			return _handle; 
		}
		virtual uint API_CALL getId() { return getHandle(); }
		virtual IntPtr API_CALL getTextureInstance() { return this; }

		inline void setHandle(GLuint value) { _handle = value; }
		float X;
		float Y;
		float W;
		float H;
	protected:
		GLuint _handle;
		GLuint _handleDefault;
	};
}

#endif /*TEXTURE_H*/
