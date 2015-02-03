#include "Color.h"

namespace graphics
{
	unsigned char Color::getB(unsigned int value)
	{
		return (unsigned char)(value & 0xff);
	}

	unsigned char Color::getG(unsigned int value)
	{
		return (unsigned char)((value >> 8) & 0xff);
	}

	unsigned char Color::getR(unsigned int value)
	{
		return (unsigned char)((value >> 16) & 0xff);
	}

	unsigned char Color::getA(unsigned int value)
	{
		return (unsigned char)(value >> 24);
	}
}
