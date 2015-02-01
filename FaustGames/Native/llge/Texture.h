#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics.h"
#include "TextureLoader.h"

namespace graphics
{
	class Texture
	{
	protected:
		GLuint _handle;
	public:
		inline const GLuint getHadler(){ return _handle; }
		virtual void create() = 0;
		virtual void cleanup() = 0;
	};

	class TextureImage2D : public Texture
	{
	public:
		TextureImage2D();
		bool Wrap;
		virtual void create();
		virtual void cleanup();
		void setData(const Image2DData *data);
	};
}

#endif /*TEXTURE_H*/
