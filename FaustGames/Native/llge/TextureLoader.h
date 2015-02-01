#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "graphics.h"

namespace graphics
{
	class Image2DData
	{
	public:
		int Width;
		int Height;
		unsigned int *Pixels;
		Image2DData(int bufferSize);
		Image2DData(int width, int height);
		~Image2DData();
	};
	
	class TextureAbstractLoader
	{
	public:
		virtual Image2DData * loadImage2DData(int id) = 0;
	};

	class TextureTestLoader : public TextureAbstractLoader
	{
	private:
		Image2DData _data;
	public:
		TextureTestLoader();
		virtual Image2DData * loadImage2DData(int id);
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
