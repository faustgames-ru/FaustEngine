#ifndef FONTS_FONT_CHAR_SET_H
#define FONTS_FONT_CHAR_SET_H

#include "fonts_classes.h"

namespace fonts
{
	class FontCharSet
	{
	public:
		static FontCharSet latin;
		static FontCharSet all;
		std::string charset;
		FontCharSet(char from, char to);
		FontCharSet(const char *chars);
	private:
	};
}

#endif /*FONTS_FONT_CHAR_SET_H*/
