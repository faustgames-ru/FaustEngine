#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics_classes.h"
#include "TextureLoader.h"

namespace graphics
{
	class TextureTransform
	{
	public:
		float x0;
		float y0;
		float xx;
		float xy;
		float yx;
		float yy;
		TextureTransform();
		TextureTransform(float x, float y, float w, float h);
		float transformU(float u, float v);
		float transformV(float u, float v);
	};

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
		virtual uint API_CALL getAlphaId() { return 0; }
		virtual IntPtr API_CALL getTextureInstance() { return this; }

		inline void setHandle(GLuint value) { _handle = value; }
		TextureTransform transform;
	protected:
		GLuint _handle;
		GLuint _handleDefault;
	};
}

#endif /*TEXTURE_H*/
