#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "graphics_classes.h"
#include "Image2dData.h"

namespace graphics
{	
	class TextureAbstractLoader
	{
	public:
		virtual Image2dData * loadImage2dData(int id) = 0;
	};

	class TextureTestLoader : public TextureAbstractLoader
	{
	private:
		Image2dData _data;
	public:
		TextureTestLoader();
		virtual Image2dData * loadImage2dData(int id);
	};

	class TextureLoader
	{
	private:
		static TextureTestLoader _testLoader;
	public:
		static TextureAbstractLoader * instance();
	};
}

#endif /*TEXTURE_LOADER_H*/
