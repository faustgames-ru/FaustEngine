#include "Color.h"

namespace graphics
{
	unsigned int Color::premul(unsigned int value, bool additive)
	{
		int r = getR(value);
		int g = getG(value);
		int b = getB(value);
		int a = getA(value);
		r *= a;
		g *= a;
		b *= a;
		r = r >> 8;
		g = g >> 8;
		b = b >> 8;
		if (additive)
		{
			a = 0;
		}
		return fromRgba((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
	}

	unsigned int Color::mulA(unsigned int value, float a)
	{
		int r = (int)core::Math::round(getR(value)*a);
		int g = (int)core::Math::round(getG(value)*a);
		int b = (int)core::Math::round(getB(value)*a);
		int alpha = (int)core::Math::round(getA(value)*a);
		return fromRgba((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)alpha);
	}


	unsigned int Color::fromRgba(float r, float g, float b, float a)
	{		
		return fromRgba(
			(unsigned char)(core::Math::saturate(r) * 255),
			(unsigned char)(core::Math::saturate(g) * 255),
			(unsigned char)(core::Math::saturate(b) * 255),
			(unsigned char)(core::Math::saturate(a) * 255)
			);
	}

	unsigned int Color::fromRgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		return (unsigned int)r + ((unsigned int)g << 8) + ((unsigned int)b << 16) + ((unsigned int)a << 24);
	}

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

	unsigned int Color::lerp(unsigned int from, unsigned int to, float u)
	{
		float a = getA(from);
		float r0 = getR(from);
		float g0 = getG(from);
		float b0 = getB(from);

		float r1 = getR(to);
		float g1 = getG(to);
		float b1 = getB(to);

		float r = core::Math::lerp(r0, r1, u);
		float g = core::Math::lerp(g0, g1, u);
		float b = core::Math::lerp(b0, b1, u);
		return fromRgba(r, g, b, a);
	}
}
