#include "Image2dData.h"
#include "../core/HollowsAllocationPolicy.h"

namespace graphics
{
	int Image2dData::getFullWidth()
	{
		return Width + BorderSize * 2;
	}

	int Image2dData::getFullHeight()
	{
		return Height + BorderSize * 2;
	}

	Image2dData::Image2dData() : Width(0), Height(0), BlocksOrder(graphics::Image2dBlocksOrder::Normal), RawDataOffset(0), Format(Image2dFormat::Rgba), Pixels(nullptr), BorderSize(0)
	{
	}

	Image2dData::Image2dData(int bufferSize) : Width(0), Height(0), BlocksOrder(graphics::Image2dBlocksOrder::Normal), RawDataOffset(0), BorderSize(0)
	{
		Format = Image2dFormat::Rgba;
		//Pixels = (unsigned int *)core::Mem::allocate(bufferSize * sizeof(unsigned int));
		Pixels = new unsigned int[bufferSize];
	}

	Image2dData::Image2dData(int width, int height) : Width(width), Height(height), BlocksOrder(graphics::Image2dBlocksOrder::Normal), RawDataOffset(0), BorderSize(0)
	{
		Format = Image2dFormat::Rgba;
		//Pixels = (unsigned int *)core::Mem::allocate(Width * Height * sizeof(unsigned int));//new unsigned int[Width * Height];
		Pixels = new unsigned int[Width * Height];
	}

	Image2dData::Image2dData(const Image2dResourceData& r)
	{
		Width = r.Width;
		Height = r.Height;
		Format = r.Format;
		BorderSize = r.BorderSize;
		BlocksOrder = r.BlocksOrder;
		RawDataOffset = r.RawDataOffset;
		Pixels = static_cast<unsigned int*>(r.Pixels);
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