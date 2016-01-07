#include "Matrix.h"
#include "Math.h"

namespace core
{
	Matrix Matrix::identity(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	int MatrixContainer::_id(0);

	Matrix2 Matrix2::identity(1, 0, 0, 1);

	Matrix Matrix::createRotation(Vector3 n, float a)
	{
		float u = n.getX();
		float v = n.getY();
		float w = n.getZ();
		float u2 = u*u;
		float v2 = v*v;
		float w2 = w*w;
		float c = Math::cos(a);
		float s = Math::sin(a);

		return Matrix(
			u2 + (1 - u2)*c, u*v*(1 - c) - w*s, u*w*(1 - c) + v*s, 0.0f,
			u*v*(1 - c) + w*s, v2 + (1 - v2)*c, v*w*(1 - c) - u*s, 0.0f,
			u*w*(1 - c) - v*s, v*w*(1 - c) + u*s, w2 + (1 - w2)*c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix2::Matrix2(
		float m11, float m12,
		float m21, float m22)
	{
		_values[0] = m11;
		_values[1] = m12;
		_values[2] = m21;
		_values[3] = m22;
	}

	float Matrix2::getXx() const { return _values[0]; }
	float Matrix2::getXy() const { return _values[1]; }

	float Matrix2::getYx() const { return _values[2]; }
	float Matrix2::getYy() const { return _values[3]; }


	Matrix2 Matrix2::mul(const Matrix2 &a, const Matrix2 &b)
	{
		return Matrix2(
			a.getXx()*b.getXx() + a.getXy()*b.getYx(), a.getXx()*b.getXy() + a.getXy()*b.getYy(),
			a.getYx()*b.getXx() + a.getYy()*b.getYx(), a.getYx()*b.getXy() + a.getYy()*b.getYy());
	}


	Matrix2 Matrix2::createRotation(float angle)
	{
		float sin = Math::sin(angle);
		float cos = Math::cos(angle);
		return Matrix2(cos, -sin, sin, cos);
	}

	Matrix2 Matrix2::createScale(float scaleX, float scaleY)
	{
		return Matrix2(scaleX, 0, 0, scaleY);
	}
}
