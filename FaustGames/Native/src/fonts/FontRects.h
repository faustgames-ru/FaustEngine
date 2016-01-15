#ifndef FONTS_FONT_RECTS_H
#define FONTS_FONT_RECTS_H

#include "fonts_classes.h"

namespace fonts
{
	class FontRectSize
	{
	public:
		char symbol;
		uint w;
		uint h;
		FontRectSize();
		FontRectSize(char symbolValue, uint width, uint height);
		bool empty() const;
	};

	class FontRect
	{
	public:
		char symbol;
		uint x;
		uint y;
		uint w;
		uint h;
		FontRect();
		FontRect(uint xValue, uint yValue, uint width, uint height);
		FontRect(char symbol, uint xValue, uint yValue, uint width, uint height);
	};

	class FontRectsPage
	{
	public:
		std::vector<FontRect> rects;
		uint w;
		uint h;
	};

	class FontRects
	{
	public:
		static void build(uint pageW, uint pageH, std::vector<FontRectSize> &sizes, std::vector<FontRectsPage> &pages);
	};
}

#endif /*FONTS_FONT_CHAR_SET_H*/