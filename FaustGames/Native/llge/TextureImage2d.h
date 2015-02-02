#ifndef TEXTURE_IMAGE_2D_H
#define TEXTURE_IMAGE_2D_H

#include "graphics.h"
#include "TextureLoader.h"
#include "Texture.h"

namespace graphics
{
	class TextureImage2d : public Texture
	{
	public:
		TextureImage2d();
		void create();
		void cleanup();
		void setData(const Image2dData *data);
	protected:
	private:
		bool _wrap;
	};
}

#endif /*TEXTURE_IMAGE_2D_H*/
