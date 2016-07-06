#ifndef SPLINE_H
#define SPLINE_H

#include "core_classes.h"
#include "Vector.h"

namespace core
{
	class line2
	{
		
	};

	class Spline2
	{
	public:
		Spline2(
			float x0, float y0,
			float x1, float y1,
			float x2, float y2,
			float x3, float y3, float t, int detail);
		~Spline2();
		float getU(float l);
		Vector2 get(float u);
		float length();
	private:
		Vector2 _p[4];
		float _t;
		float* _len;
		int _latLenIndex;
		int _detail;
	};

	struct MSplineU
	{
		Spline2* spline;
		float u;
	};

	class MacroSpline2
	{
	public:
		MacroSpline2();
		~MacroSpline2();
		void addSpline(
			float x0, float y0,
			float x1, float y1,
			float x2, float y2,
			float x3, float y3, float t, int detail);
		MSplineU getU(float l);
		Vector2 get(MSplineU u);
		float length();
	private:
		std::vector<Spline2 *> _splines;
		float _len;
	};

}

#endif /*SPLINE_H*/