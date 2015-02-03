#ifndef IMAGE_2D_DATA_H
#define IMAGE_2D_DATA_H

#include "graphics_classes.h"

namespace graphics
{
	class Image2dData
	{
	public:
		int Width;
		int Height;
		unsigned int *Pixels;
		Image2dData(int bufferSize);
		Image2dData(int width, int height);
		~Image2dData();
	};
}

#endif /*IMAGE_2D_DATA_H*/
