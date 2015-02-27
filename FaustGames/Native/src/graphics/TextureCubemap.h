#ifndef TEXTURE_CUBEMAP_H
#define TEXTURE_CUBEMAP_H

#include "graphics_classes.h"
#include "TextureLoader.h"
#include "Texture.h"

namespace graphics
{
	struct TextureCubemapPlanes
	{
		enum e
		{
			PositiveX = 0,
			NegativeX = 2,
			PositiveY = 3,
			NegativeY = 4,
			PositiveZ = 5,
			NegativeZ = 6,
		};
	};

	class TextureCubemap : public Texture
	{
	public:
		TextureCubemap();
		void create();
		void cleanup();
		void setData(TextureCubemapPlanes::e plane, const Image2dData *data);
	protected:
	private:
		bool _wrap;
		bool _filter;
	};
}

#endif /*TEXTURE_CUBEMAP_H*/
