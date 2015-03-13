#include "Image2dData.h"

namespace graphics
{
	Image2dData::Image2dData(int bufferSize)
	{
		Pixels = new unsigned int[bufferSize];
	}

	Image2dData::Image2dData(int width, int height) : Width(width), Height(height)
	{
		Pixels = new unsigned int[Width * Height];
	}

	void Image2dData::realloc(int bufferSize)
	{
		if (Pixels)
		{
			delete[] Pixels;
		}
		Pixels = new unsigned int[bufferSize];
	}

	Image2dData::~Image2dData()
	{
		if (Pixels)
		{
			delete[] Pixels;
		}
	}

}