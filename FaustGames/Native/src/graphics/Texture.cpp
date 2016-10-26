#include "Texture.h"
#include "Errors.h"

namespace graphics
{
	TextureTransform::TextureTransform()
	{		  
		x0 = 0.0f;
		y0 = 0.0f;
		xx = 1.0f;
		xy = 0.0f;
		yx = 0.0f;
		yy = 1.0f;
	}

	TextureTransform::TextureTransform(float x, float y, float w, float h)
	{
		x0 = x;
		y0 = y;
		xx = w;
		xy = 0.0f;
		yx = 0.0f;
		yy = h;
	}

	float TextureTransform::transformU(float u, float v)
	{
		return x0 + xx*u + xy*v;
	}

	float TextureTransform::transformV(float u, float v)
	{
		return y0 + yx*u + yy*v;
	}
}