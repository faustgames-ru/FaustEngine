#include "FontRects.h"
#include "../../src_rectanglebinpack/MaxRectsBinPack.h"
#include "../../src_rectanglebinpack/SkylineBinPack.h"

namespace fonts
{
	FontRectSize::FontRectSize(): symbol(0), w(0), h(0)
	{
	}

	FontRectSize::FontRectSize(char symbolValue, uint width, uint height): symbol(symbolValue), w(width), h(height)
	{
	}

	bool FontRectSize::empty() const
	{
		return (w == 0) || (h == 0);
	}

	FontRect::FontRect(): symbol(0), x(0), y(0), w(0), h(0)
	{
	}

	FontRect::FontRect(uint xValue, uint yValue, uint width, uint height)
		: symbol(0), x(xValue), y(yValue), w(width), h(height)
	{
	}

	FontRect::FontRect(char symbol, uint xValue, uint yValue, uint width, uint height)
		: symbol(symbol), x(xValue), y(yValue), w(width), h(height)
	{
	}

	void FontRects::build(uint pageW, uint pageH, std::vector<FontRectSize>& sizes, std::vector<FontRectsPage>& pages)
	{
		std::vector<rbp::RectSize> packSizes;
		for (uint i = 0; i < sizes.size(); i++)
		{
			rbp::RectSize size;
			size.width = sizes[i].w;
			size.height = sizes[i].h;
			packSizes.push_back(size);
		}
		std::vector<rbp::Rect> packRects;
		while (!packSizes.empty())
		{
			rbp::MaxRectsBinPack pack(pageW, pageH);
			pack.Insert(packSizes, packRects, rbp::MaxRectsBinPack::RectBestShortSideFit);
			pages.push_back(FontRectsPage());
			FontRectsPage &p = pages.back();
			p.w = 0;
			p.h = 0;
			p.rects.resize(packRects.size());
			for (uint i = 0; i < packRects.size(); i++)
			{
				uint w = packRects[i].x + packRects[i].width;
				uint h = packRects[i].y + packRects[i].height;
				if (p.w < w)
				{
					p.w = w;
				}
				if (p.h < h)
				{
					p.h = h;
				}
				p.rects[i].x = packRects[i].x;
				p.rects[i].y = packRects[i].y;
				p.rects[i].w = packRects[i].width;
				p.rects[i].h = packRects[i].height;
			}
			packRects.clear();
		}

		for (uint i = 0; i < pages.size(); i++)
		{
			FontRectsPage& p = pages[i];
			for (uint j = 0; j < p.rects.size(); j++)
			{
				FontRect& r = p.rects[j];
				bool notFound = true;
				for (std::vector<FontRectSize>::iterator k = sizes.begin(); k != sizes.end(); k++)
				{
					if (k->w == r.w && k->h == r.h ||
						k->w == r.h && k->h == r.w)
					{
						p.rects[j].symbol = k->symbol;
						sizes.erase(k);
						notFound = false;
						break;
					}
				}
				if (notFound)
				{
					// error
					notFound = true;
				}
			}
		}

		// fill pages symbols
	}
}
