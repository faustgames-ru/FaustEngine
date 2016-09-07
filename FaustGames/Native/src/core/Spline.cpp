#include "Spline.h"

namespace core
{
	Spline2::Spline2(
		float x0, float y0, float t0,
		float x1, float y1, float t1,
		float x2, float y2, float t2,
		float x3, float y3, float t3, int detail)
	{
		if (detail < 3)
			detail = 3;

		_detail = detail;
		_p[0].setX(x0);
		_p[0].setY(y0);
		_t[0] = t0;
		_p[1].setX(x1);
		_p[1].setY(y1);
		_t[1] = t1;
		_p[2].setX(x2);
		_p[2].setY(y2);
		_t[2] = t2;
		_p[3].setX(x3);
		_p[3].setY(y3);
		_t[3] = t3;
		_len = new float[detail];
		Vector2 p0 = get(0);
		float l = 0;
		_len[0] = l;
		for (uint i =1; i < detail; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(detail-1);
			Vector2 p1 = get(u);
			l += (p1 - p0).length();
			_len[i] = l;
			p0 = p1;
		}
		_latLenIndex = 0;
	}

	Spline2::~Spline2()
	{
		delete [] _len;
	}

	float Spline2::getU(float l)
	{
		float minL = _len[_latLenIndex];
		float maxL;
		if (l < minL)
		{
			_latLenIndex = 0;
		}

		for (int i = _latLenIndex+1; i < _detail; i++)
		{
			maxL = _len[i];
			if (l < maxL)
			{
				_latLenIndex = i - 1;
				float u0 = static_cast<float>(i - 1) / static_cast<float>(_detail - 1);
				float u1 = static_cast<float>(i) / static_cast<float>(_detail - 1);

				float uu = (l - _len[i-1]) / (_len[i] - _len[i-1]);
				float u = Math::lerp(u0, u1, uu);
				return u;
			}
		}
		return 1.0f;
	}

	Vector2 Spline2::get(float u)
	{
		if (u < 0.001f)
		{
			return _p[1];
		}
		if (u > 0.999f)
		{
			return _p[2];
		}
		return Vector2(
			Math::spline(_p[0].getX(), _p[1].getX(), _p[2].getX(), _p[3].getX(), u, _t[1], _t[2]),
			Math::spline(_p[0].getY(), _p[1].getY(), _p[2].getY(), _p[3].getY(), u, _t[1], _t[2]));
	}

	float Spline2::length()
	{
		return _len[_detail - 1];
	}

	MacroSpline2::MacroSpline2(): _len(0.0f)
	{
	}

	MacroSpline2::~MacroSpline2()
	{
		for (int i = 0; i < _splines.size(); i++)
			delete _splines[i];
	}

	void MacroSpline2::addSpline(
		float x0, float y0, float t0,
		float x1, float y1, float t1,
		float x2, float y2, float t2,
		float x3, float y3, float t3, int detail)
	{
		Spline2 *spline = new Spline2(
			x0, y0, t0,
			x1, y1, t1,
			x2, y2, t2,
			x3, y3, t3, 
			detail);
		_len += spline->length();
		_splines.push_back(spline);
	}

	MSplineU MacroSpline2::getU(float l)
	{
		float lmin = 0.0f;
		MSplineU result;
		for (int i = 0; i < _splines.size(); i++)
		{
			float splineL = _splines[i]->length();
			float locaL = l - lmin;
			if (locaL <= splineL)
			{
				result.spline = _splines[i];
				result.u = result.spline->getU(locaL);
				return result;
			}
			lmin += splineL;
		}
		result.spline = _splines.back();
		result.u = 1.0f;
		return result;
	}

	float MacroSpline2::length()
	{
		return _len;
	}

	Vector2 MacroSpline2::get(MSplineU u)
	{
		return u.spline->get(u.u);
	}
}