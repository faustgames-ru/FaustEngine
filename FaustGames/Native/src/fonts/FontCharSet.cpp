#include "FontCharSet.h"

namespace fonts
{
	FontCharSet FontCharSet::latin(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
	FontCharSet FontCharSet::all(' ', 127);


	FontCharSet::FontCharSet(char from, char to)
	{
		charset.resize(to - from + 1);
		for (char c = from; c < to; c++)
		{
			charset[c - from] = c;
		}
	}

	FontCharSet::FontCharSet(const char* chars)
	{
		charset = chars;
	}
}
