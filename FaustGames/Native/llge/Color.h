#ifndef COLOR_H
#define COLOR_H

#include "graphics.h"

namespace graphics
{
	class Color
	{
	public:
		static unsigned char getR(unsigned int value);
		static unsigned char getG(unsigned int value);
		static unsigned char getB(unsigned int value);
		static unsigned char getA(unsigned int value);
	};
}

#endif /*COLOR_H*/
