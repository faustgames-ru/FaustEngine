#ifndef SPLINE_H
#define SPLINE_H

#include "core_classes.h"
#include "Vector.h"

namespace core
{
	class Spline2
	{
	public:
		Spline2(
			float x0, float y0, float t0,
			float x1, float y1, float t1,
			float x2, float y2, float t2,
			float x3, float y3, float t3, int detail);
		~Spline2();
		float getU(float l);
		Vector2 get(float u);
		float length();
	private:
		Vector2 _p[4];
		float _t[4];
		float* _len;
		int _latLenIndex;
		int _detail;
	};
}

#endif /*SPLINE_H*/