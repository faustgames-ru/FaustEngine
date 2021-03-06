#include "Math.h"

namespace core
{
	float Math::Epsilon(1e-8f);
	float Math::MaxValue(1e10f);
	float Math::Pi(3.14159265359f);

	float Math::abs(float a)
	{
		return a > 0 ? a : -a;
	}

	float Math::saturate(float a)
	{
		if (a < 0.0f)
			return 0.0f;
		if (a > 1.0f)
			return 1.0f;
		return a;
	}

	float Math::trunc(float a)
	{
		return ::trunc(a);
	}

	float Math::round(float a)
	{
		return ::round(a);
	}

	float Math::cos(float a)
	{
		return ::cos(a);
	}

	float Math::sin(float a)
	{
		return ::sin(a);
	}

	float Math::tan(float a)
	{
		return ::tan(a);
	}

	float Math::sqrt(float x)
	{
		return ::sqrt(x);
	}

	float Math::acos(float v)
	{
		return ::acos(v);
	}

	float Math::atan(float v)
	{
		return ::atan(v);
	}

	float Math::atan2(float y, float x)
	{
		return ::atan2(y, x);
	}

	float Math::lerp(float from, float to, float u)
	{
		return from + u * (to - from);
	}

	float Math::conic(float p0, float p1, float p2, float u)
	{
		return lerp(lerp(p0, p1, u), lerp(p1, p2, u), u);
	}

	float Math::cubic(float p0, float p1, float p2, float p3, float u)
	{
		return conic(lerp(p0, p1, u), lerp(p1, p2, u), lerp(p2, p3, u), u);
	}

	float Math::spline(float p1, float p2, float p3, float p4, float u, float s)
	{
		return
			p1*(-s*u*u*u + 2 * s*u*u - s*u) +
			p2*((2 - s)*u*u*u + (s - 3)*u*u + 1) +
			p3*((s - 2)*u*u*u + (3 - 2 * s)*u*u + s*u) +
			p4*(s*u*u*u - s*u*u);
	}

	float Math::spline(float p1, float p2, float p3, float p4, float u, float s2, float s3)
	{
		float bp1 = p2;
		float bp4 = p3;

		float bp2 = p2 + (p3 - p1)* s2 / 3.0f;
		float bp3 = p3 + (p2 - p4)* s3 / 3.0f;
		return cubic(bp1, bp2, bp3, bp4, u);
	}

	float Math::asin(float v)
	{
		return ::asin(v);
	}

	float Math::frac(float v)
	{
		return v - trunc(v);
	}

	float Math::max(float v0, float v1)
	{
		return v0 > v1 ? v0 : v1;
	}

	float Math::max(float v0, float v1, float v2)
	{
		return max(max(v0, v1), v2);
	}

	float Math::min(float v0, float v1)
	{
		return v0 < v1 ? v0 : v1;
	}

	float Math::exp(float v)
	{
		return ::exp(v);
	}

	float Math::fmod(float x, float d)
	{
		return ::fmod(x, d);
	}

	float Math::clamp(float x, float from, float to)
	{
		if (x < from)
			return from;
		if (x > to)
			return to;
		return x;
	}

	float Math::clampi(int x, int from, int to)
	{
		if (x < from)
			return from;
		if (x > to)
			return to;
		return x;
	}

	int Math::pot(int value)
	{
		int v = 1;
		while (v < value)
		{
			v <<= 1;
		}
		return v;
	}

	int Math::align(int value, int align)
	{
		int result = (value / align)*align;
		if (result < value)
			result += align;
		return result;
	}

	uint Math::mortonCode(uint x, uint y)
	{
		return morton2DSplitBy2Bits(x) | (morton2DSplitBy2Bits(y) << 1);
	}

	void Math::mortonDecode(uint m, uint& x, uint& y)
	{
		x = mortonDecodeX(m);
		y = mortonDecodeY(m);
	}

	uint Math::mortonDecodeX(uint m)
	{
		return morton2DGetSecondBits(m);
	}

	uint Math::mortonDecodeY(uint m)
	{
		return morton2DGetSecondBits(m >> 1);
	}

	uint Math::morton2DSplitBy2Bits(uint a)
	{
		uint* masks = Magicbit2DMasks32;
		uint x = a;
		x = (x | x << 16) & masks[1];
		x = (x | x << 8) & masks[2];
		x = (x | x << 4) & masks[3];
		x = (x | x << 2) & masks[4];
		x = (x | x << 1) & masks[5];
		return x;
	}

	uint Math::morton2DGetSecondBits(uint m)
	{
		uint* masks = Magicbit2DMasks32;
		uint x = m & masks[4];
		x = (x ^ (x >> 2)) & masks[3];
		x = (x ^ (x >> 4)) & masks[2];
		x = (x ^ (x >> 8)) & masks[1];
		return x;
	}

	bool Math::equals(float a, float b, float epsilon)
	{
		float d = a - b;
		return (d > -epsilon) && (d < epsilon);
	}

	uint Math::Magicbit2DMasks32[] = { 0xFFFFFFFF, 0x0000FFFF, 0x00FF00FF, 0x0F0F0F0F, 0x33333333, 0x55555555 };
}