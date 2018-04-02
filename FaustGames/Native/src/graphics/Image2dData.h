#ifndef IMAGE_2D_DATA_H
#define IMAGE_2D_DATA_H

#include "graphics_classes.h"

namespace graphics
{
	struct Image2dResourceData;

	class Image2dData
	{
	public:
		int Width;
		int Height;
		int BorderSize;
		Image2dBlocksOrder::e BlocksOrder;
		int RawDataOffset;
		Image2dFormat::e Format;
		unsigned int *Pixels;

		int getFullWidth();
		int getFullHeight();
		Image2dData();
		Image2dData(int bufferSize);
		Image2dData(int width, int height);
		explicit Image2dData(const Image2dResourceData &source);
		void realloc(int bufferSize);
		~Image2dData();
	};

	struct Image2dResourceData
	{
		int Width;
		int Height;
		int BorderSize;
		int RawDataOffset;
		Image2dBlocksOrder::e BlocksOrder;
		Image2dFormat::e Format;
		unsigned int *Pixels;
	};

}

#endif /*IMAGE_2D_DATA_H*/
