#ifndef COLOR_H
#define COLOR_H

#include "graphics_classes.h"

namespace graphics
{
	class Color
	{
	public:
		static unsigned int premul(unsigned int value, bool additive);
		static unsigned int fromRgba(float r, float g, float b, float a);
		static unsigned int fromRgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		static unsigned char getR(unsigned int value);
		static unsigned char getG(unsigned int value);
		static unsigned char getB(unsigned int value);
		static unsigned char getA(unsigned int value);
		static float getRf(unsigned int value);
		static float getGf(unsigned int value);
		static float getBf(unsigned int value);
		static float getAf(unsigned int value);
	protected:
	private:
	};

}

#endif /*COLOR_H*/
