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
	int Matrix3Container::_id(0);

	Matrix2 Matrix2::identity(1, 0, 0, 1);

	Matrix Matrix::toSkyboxProjection() const
	{
		return Matrix(
			getXx(), getXy(), getXz(), 0,
			getYx(), getYy(), getYz(), 0,
			getZx(), getZy(), getZz(), 0,
			0, 0, 0, 1.0f);
	}
    
    float Matrix::getXx() const { return _values[0]; }
    float Matrix::getXy() const { return _values[1]; }
    float Matrix::getXz() const { return _values[2]; }
    float Matrix::getXw() const { return _values[3]; }
    
    float Matrix::getYx() const { return _values[4]; }
    float Matrix::getYy() const { return _values[5]; }
    float Matrix::getYz() const { return _values[6]; }
    float Matrix::getYw() const { return _values[7]; }
    
    float Matrix::getZx() const { return _values[8]; }
    float Matrix::getZy() const { return _values[9]; }
    float Matrix::getZz() const { return _values[10]; }
    float Matrix::getZw() const { return _values[11]; }
    
    float Matrix::getWx() const { return _values[12]; }
    float Matrix::getWy() const { return _values[13]; }
    float Matrix::getWz() const { return _values[14]; }
    float Matrix::getWw() const { return _values[15]; }

	bool Matrix::equals(const Matrix& a, const Matrix& b)
	{
		{
			return
				Math::equals(a.getXx(), b.getXx(), Math::Epsilon) &&
				Math::equals(a.getXy(), b.getXy(), Math::Epsilon) &&
				Math::equals(a.getXz(), b.getXz(), Math::Epsilon) &&
				Math::equals(a.getXw(), b.getXw(), Math::Epsilon) &&

				Math::equals(a.getYx(), b.getYx(), Math::Epsilon) &&
				Math::equals(a.getYy(), b.getYy(), Math::Epsilon) &&
				Math::equals(a.getYz(), b.getYz(), Math::Epsilon) &&
				Math::equals(a.getYw(), b.getYw(), Math::Epsilon) &&

				Math::equals(a.getZx(), b.getZx(), Math::Epsilon) &&
				Math::equals(a.getZy(), b.getZy(), Math::Epsilon) &&
				Math::equals(a.getZz(), b.getZz(), Math::Epsilon) &&
				Math::equals(a.getZw(), b.getZw(), Math::Epsilon) &&

				Math::equals(a.getWx(), b.getWx(), Math::Epsilon) &&
				Math::equals(a.getWy(), b.getWy(), Math::Epsilon) &&
				Math::equals(a.getWz(), b.getWz(), Math::Epsilon) &&
				Math::equals(a.getWw(), b.getWw(), Math::Epsilon);
		}
	}

	Matrix Matrix::createScale(float x, float y, float z)
	{
		return Matrix(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

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

	Matrix Matrix::CreateRotationWithScale(const Vector3 n, const Vector3 scale, float a)
	{
		float u = n.getX();
		float v = n.getY();
		float w = n.getZ();
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		Matrix Txz = Matrix(
			u / Math::sqrt(u2 + v2), v / Math::sqrt(u2 + v2), 0, 0,
			-v / Math::sqrt(u2 + v2), u / Math::sqrt(u2 + v2), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		Matrix Tz = Matrix(
			w / Math::sqrt(u2 + v2 + w2), 0, -Math::sqrt(u2 + v2) / Math::sqrt(u2 + v2 + w2), 0,
			0, 1, 0, 0,
			Math::sqrt(u2 + v2) / Math::sqrt(u2 + v2 + w2), 0, w / Math::sqrt(u2 + v2 + w2), 0,
			0, 0, 0, 1);

		Matrix Txzi = Txz.inverse();
		Matrix Tzi = Tz.inverse();

		Matrix Rz = createRotation(Vector3::eZ, a);
		Matrix Sxyz = createScale(scale.getX(), scale.getY(), scale.getZ());
		return mul(Txzi, Tzi, Sxyz, Rz, Tz, Txz);
	}

	Matrix3::Matrix3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
	{
		_values[0] = m11;
		_values[1] = m12;
		_values[2] = m13;

		_values[3] = m21;
		_values[4] = m22;
		_values[5] = m23;

		_values[6] = m31;
		_values[7] = m32;
		_values[8] = m33;
	}

	Matrix3::Matrix3(Matrix value)
	{
		_values[0] = value.getXx();
		_values[1] = value.getXy();
		_values[2] = value.getXz();

		_values[3] = value.getYx();
		_values[4] = value.getYy();
		_values[5] = value.getYz();

		_values[6] = value.getZx();
		_values[7] = value.getZy();
		_values[8] = value.getZz();
	}

	float Matrix3::getXx() const
	{
		return _values[0];
	}

	float Matrix3::getXy() const
	{
		return _values[1];
	}

	float Matrix3::getXz() const
	{
		return _values[2];
	}

	float Matrix3::getYx() const
	{
		return _values[3];
	}

	float Matrix3::getYy() const
	{
		return _values[4];
	}

	float Matrix3::getYz() const
	{
		return _values[5];
	}

	float Matrix3::getZx() const
	{
		return _values[6];
	}

	float Matrix3::getZy() const
	{
		return _values[7];
	}

	float Matrix3::getZz() const
	{
		return _values[8];
	}

	bool Matrix3::isIdentity()
	{
		for (int i = 0; i < 9; i++)
		{
			if (!Math::equals(_values[i], identity._values[i], 0.001f))
			{
				return false;
			}
		}
		return true;
	}

	Matrix3 Matrix3::identity(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);


	Matrix3 Matrix3::createRotation(const Vector3 n, float a)
	{
		float u = n.getX();
		float v = n.getY();
		float w = n.getZ();
		float u2 = u*u;
		float v2 = v*v;
		float w2 = w*w;
		float c = Math::cos(a);
		float s = Math::sin(a);

		return Matrix3(
			u2 + (1 - u2)*c, u*v*(1 - c) - w*s, u*w*(1 - c) + v*s,
			u*v*(1 - c) + w*s, v2 + (1 - v2)*c, v*w*(1 - c) - u*s,
			u*w*(1 - c) - v*s, v*w*(1 - c) + u*s, w2 + (1 - w2)*c);
	}

	Matrix3 Matrix3::createRotation(const Vector3 n, float c, float s)
	{
		float u = n.getX();
		float v = n.getY();
		float w = n.getZ();
		float u2 = u*u;
		float v2 = v*v;
		float w2 = w*w;

		return Matrix3(
			u2 + (1 - u2)*c, u*v*(1 - c) - w*s, u*w*(1 - c) + v*s,
			u*v*(1 - c) + w*s, v2 + (1 - v2)*c, v*w*(1 - c) - u*s,
			u*w*(1 - c) - v*s, v*w*(1 - c) + u*s, w2 + (1 - w2)*c);
	}

	Matrix3 Matrix3::createRotation(const Vector3 n, float scale, float c, float s)
	{
		float u = n.getX();
		float v = n.getY();
		float w = n.getZ();
		float u2 = u*u;
		float v2 = v*v;
		float w2 = w*w;

		return Matrix3(
			(u2 + (1 - u2)*c) * scale, (u*v*(1 - c) - w*s)*scale, (u*w*(1 - c) + v*s)*scale,
			(u*v*(1 - c) + w*s)*scale, (v2 + (1 - v2)*c)*scale,(v*w*(1 - c) - u*s)*scale,
			(u*w*(1 - c) - v*s)*scale, (v*w*(1 - c) + u*s)*scale, (w2 + (1 - w2)*c)*scale);
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

	Matrix2 Matrix2::inverse() const
	{
		float d = getXx() * getYy() - getXy() * getYx();
		if (d < Math::Epsilon)
			return *this;
		d = 1.0f / d;
		return Matrix2(getYy() * d, -getXy() * d, -getYx() * d, getXx() * d);
	}

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

	Matrix3Container::Matrix3Container()
	{
		id = (++_id) << 16;
	}

	Matrix3Container::Matrix3Container(const Matrix3& value)
	{
		id = (++_id) << 16;
		Value = value;
	}

	void Matrix3Container::setValue(const Matrix3& value)
	{
		++id;
		Value = value;
	}

	void Matrix3Container::setValue(const float* value)
	{
		++id;
		Value = *(static_cast<const Matrix3 *>(static_cast<const void *>(value)));
	}

	Quaternion::Quaternion()
	{
		_values[0] = 0;
		_values[1] = 0;
		_values[2] = 0;
		_values[3] = 1;
	}

	Quaternion::Quaternion(Vector3 axis, float angle)
	{
		float s = Math::sin(angle / 2);
		float c = Math::cos(angle / 2);
		_values[0] = axis.getX() * s;
		_values[1] = axis.getY() * s;
		_values[2] = axis.getZ() * s;
		_values[3] = c;
	}

	Quaternion::Quaternion(float x, float y, float z, float angle)
	{
		float s = Math::sin(angle / 2);
		float c = Math::cos(angle / 2);
		_values[0] = x * s;
		_values[1] = y * s;
		_values[2] = z * s;
		_values[3] = c;
	}

	Vector3 Quaternion::transform(const Quaternion& q, const Vector3& v)
	{
		Vector3 u(q.getX(), q.getY(), q.getZ());
		float s = q.getW();
		return u * (Vector3::dotProduct(u, v)*2.0f)
			+ v*(s*s - Vector3::dotProduct(u, u))
			+ Vector3::crossProduct(u, v) * (s *2.0f);
	}
}
