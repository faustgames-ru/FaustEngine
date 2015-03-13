#ifndef TEXTURE_IMAGE_2D_H
#define TEXTURE_IMAGE_2D_H

#include "graphics_classes.h"
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
		bool _createMipmaps;
		bool _wrap;
		bool _filter;
	};
}

#endif /*TEXTURE_IMAGE_2D_H*/
