#ifndef ZOMBO_BITMAP_FONT_H
#define ZOMBO_BITMAP_FONT_H

#include "../zombo_classes.h"
#include "../../fonts/BitmapFont.h"

namespace zombo
{
	class ZomboBitmapFont
	{
	public:
		fonts::BitmapFont* font;
		static ZomboBitmapFont* create();
	};
}

#endif /*ZOMBO_BITMAP_FONT_H*/

