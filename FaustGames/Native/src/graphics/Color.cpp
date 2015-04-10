#include "Color.h"

namespace graphics
{
	unsigned char Color::getR(unsigned int value)
	{
		return (unsigned char)(value & 0xff);
	}

	unsigned char Color::getG(unsigned int value)
	{
		return (unsigned char)((value >> 8) & 0xff);
	}

	unsigned char Color::getB(unsigned int value)
	{
		return (unsigned char)((value >> 16) & 0xff);
	}

	unsigned char Color::getA(unsigned int value)
	{
		return (unsigned char)(value >> 24);
	}

	float Color::getRf(unsigned int value)
	{
		return (float)getR(value) / 255.0f;
	}
	
	float Color::getGf(unsigned int value)
	{
		return (float)getG(value) / 255.0f;
	}
	
	float Color::getBf(unsigned int value)
	{
		return (float)getB(value) / 255.0f;
	}
	
	float Color::getAf(unsigned int value)
	{
		return (float)getA(value) / 255.0f;
	}

}
