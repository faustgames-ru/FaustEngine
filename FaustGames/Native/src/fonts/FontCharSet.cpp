#include "FontCharSet.h"

namespace fonts
{
	FontCharSet FontCharSet::latin(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");


	FontCharSet::FontCharSet(const char* chars)
	{
		charset = chars;
	}
}
