#ifndef COLOR_H
#define COLOR_H

#include "graphics.h"

namespace graphics
{
	class Color
	{
		int _v;
	public:
		Color(int v);
		~Color();
		static unsigned char getR(unsigned int value);
		static unsigned char getG(unsigned int value);
		static unsigned char getB(unsigned int value);
		static unsigned char getA(unsigned int value);

		static Color Default;
	};
}

#endif /*COLOR_H*/
