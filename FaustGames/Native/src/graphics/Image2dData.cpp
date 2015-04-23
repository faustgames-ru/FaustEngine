#include "Image2dData.h"
#include "..\core\HollowsAllocationPolicy.h"

namespace graphics
{
	Image2dData::Image2dData(int bufferSize)
	{
		Format = Image2dFormat::Rgba;
		//Pixels = (unsigned int *)core::Mem::allocate(bufferSize * sizeof(unsigned int));
		Pixels = new unsigned int[bufferSize];
	}

	Image2dData::Image2dData(int width, int height) : Width(width), Height(height)
	{
		Format = Image2dFormat::Rgba;
		//Pixels = (unsigned int *)core::Mem::allocate(Width * Height * sizeof(unsigned int));//new unsigned int[Width * Height];
		Pixels = new unsigned int[Width * Height];
	}

	void Image2dData::realloc(int bufferSize)
	{
		if (Pixels)
		{
			//core::Mem::deallocate(Pixels); 
			delete[] Pixels;
		}
		Pixels = new unsigned int[bufferSize];
	}

	Image2dData::~Image2dData()
	{
		if (Pixels)
		{
			//core::Mem::deallocate(Pixels);
			delete[] Pixels;
		}
	}

}