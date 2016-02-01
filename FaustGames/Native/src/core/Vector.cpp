#include "Vector.h"

namespace core
{
	Vector2 Vector2::empty(0.0f, 0.0f);

	Vector3 Vector3::empty(0.0f, 0.0f, 0.0f);

	int Vector2::iX = 0;
	int Vector2::iY = 1;


	Vector2::Vector2()
	{}

	Vector2::Vector2(float v)
	{
		_values[0] = v;
		_values[1] = v;
	}

	Vector2::Vector2(float x, float y)
	{
		_values[0] = x;
		_values[1] = y;
	}

	Vector2& Vector2::operator+=(const Vector2& right) 
	{
		_values[iX] += right.getX();
		_values[iY] += right.getY();
		return *this;
	}

	Vector2& Vector2::operator+=(float right)
	{
		_values[iX] += right;
		_values[iY] += right;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& right)
	{
		_values[iX] -= right.getX();
		_values[iY] -= right.getY();
		return *this;
	}

	Vector2& Vector2::operator*=(const Vector2& right)
	{
		_values[iX] *= right.getX();
		_values[iY] *= right.getY();
		return *this;
	}

	Vector2& Vector2::operator*=(float right)
	{
		_values[iX] *= right;
		_values[iY] *= right;
		return *this;
	}

	Vector2& Vector2::operator/=(const Vector2& right)
	{
		_values[iX] /= right.getX();
		_values[iY] /= right.getY();
		return *this;
	}

	Vector2& Vector2::operator/=(float right)
	{
		_values[iX] /= right;
		_values[iY] /= right;
		return *this;
	}

	Vector2 Vector2::operator+(const Vector2& right) const
	{
		return Vector2(_values[iX] + right.getX(), _values[iY] + right.getY());
	}

	Vector2 Vector2::operator-(const Vector2& right) const
	{
		return Vector2(_values[iX] - right.getX(), _values[iY] - right.getY());
	}

	Vector2 Vector2::operator*(float right) const
	{
		return Vector2(_values[iX] * right, _values[iY] * right);
	}

	Vector2 Vector2::operator*(const Vector2& right) const
	{
		return Vector2(_values[iX] * right.getX(), _values[iY] * right.getY());
	}

	Vector2 Vector2::operator/(float right) const
	{
		return Vector2(_values[iX] / right, _values[iY] / right);
	}

	Vector2 Vector2::operator/(const Vector2& right) const
	{
		return Vector2(_values[iX] / right.getX(), _values[iY] / right.getY());
	}

	float Vector2::operator[](int i) const
	{
		return _values[i];
	}

	float& Vector2::operator[](int i)
	{
		return _values[i];
	}

	const float* Vector2::getData() const
	{
		return _values;
	}

	float Vector2::getX() const
	{
		return _values[0];
	}

	float Vector2::getY() const
	{
		return _values[1];
	}

	void Vector2::setX(float value)
	{
		_values[0] = value;
	}

	void Vector2::setY(float value)
	{
		_values[1] = value;
	}

	void Vector2::inverseY()
	{
		_values[1] = -_values[1];
	}

	void Vector2::inverseX()
	{
		_values[0] = -_values[0];
	}

	Vector2 Vector2::rotate90cw() const
	{
		return Vector2(getY(), -getX());
	}

	Vector2 Vector2::rotate90ccw() const
	{
		return Vector2(-getY(), getX());
	}

	Vector2 Vector2::normalize() const
	{
		float l = length();
		return Vector2(getX() / l, getY() / l);
	}

	float Vector2::length() const
	{
		return Math::sqrt(getX()*getX() + getY()*getY());
	}

	float Vector2::lengthSqr() const
	{
		return getX()*getX() + getY()*getY();
	}

	Vector3 Vector2::toVector3() const
	{
		return Vector3(getX(), getY(), 0.0f);
	}

	Vector3 Vector2::toVector3(float z) const
	{
		return Vector3(getX(), getY(), z);
	}

	Vector2 Vector2::lerp(const Vector2& p1, const Vector2& p2, float u)
	{
		return Vector2(
			Math::lerp(p1.getX(), p2.getX(), u),
			Math::lerp(p1.getY(), p2.getY(), u));
	}

	Vector2 Vector2::cubic(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float u)
	{
		return Vector2(
			Math::cubic(p0.getX(), p1.getX(), p2.getX(), p3.getX(), u),
			Math::cubic(p0.getY(), p1.getY(), p2.getY(), p3.getY(), u));
	}

	float Vector2::crossProduct(Vector2 v1, Vector2 v2)
	{
		return v1.getX()*v2.getY() - v1.getY()*v2.getX();
	}

	float Vector2::dotProduct(Vector2 v1, Vector2 v2)
	{
		return v1.getX()*v2.getX() + v1.getY()*v2.getY();
	}

	bool Vector2::equals(const Vector2& a, const Vector2& b)
	{
		return
			core::Math::equals(a.getX(), b.getX()) &&
			core::Math::equals(a.getY(), b.getY());
	}

	float Vector2::distanceToEdge(const Vector2& p, const Vector2& e0, const Vector2& e1)
	{
		const float l2 = (e1 - e0).lengthSqr();
		if (Math::equals(l2, 0.0f)) return (p - e0).length();
		const float t = dotProduct(p - e0, e1 - e0) / l2;
		if (t < 0.0) return (p - e0).length();
		if (t > 1.0) return (p - e1).length();
		Vector2 projection = e0 + (e1 - e0)*t;
		return (p - projection).length();
	}

	Vector3::Vector3(float v)
	{
		_values[0] = v;
		_values[1] = v;
		_values[2] = v;
	}

	Vector3 Vector3::eX(1.0f, 0.0f, 0.0f);
	Vector3 Vector3::eY(0.0f, 1.0f, 0.0f);
	Vector3 Vector3::eZ(0.0f, 0.0f, 1.0f);

	int Vector3::iX = 0;
	int Vector3::iY = 1;
	int Vector3::iZ = 2;


	float Vector3::operator[](int i) const
	{
		return _values[i];
	}

	float& Vector3::operator[](int i)
	{
		return _values[i];
	}
	
	/*
	Vector3& Vector3::operator=(const Vector3& right)
	{
		_values[iX] += right.getX();
		_values[iY] += right.getY();
		_values[iZ] += right.getZ();
		return *this;
	}
	*/
	Vector3& Vector3::operator+=(const Vector2& right)
	{
		_values[iX] += right.getX();
		_values[iY] += right.getY();
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector2& right)
	{
		_values[iX] -= right.getX();
		_values[iY] -= right.getY();
		return *this;
	}

	Vector3& Vector3::operator*=(float right)
	{
		_values[iX] *= right;
		_values[iY] *= right;
		_values[iZ] *= right;
		return *this;
	}

	Vector3 Vector3::operator+(const Vector3& right) const
	{
		return Vector3(
			_values[iX] + right.getX(),
			_values[iY] + right.getY(),
			_values[iZ] + right.getZ());
	}

	Vector3 Vector3::operator+(const Vector2& right) const
	{
		return Vector3(
			_values[iX] + right.getX(),
			_values[iY] + right.getY(),
			_values[iZ]);
	}

	Vector3 Vector3::operator-(const Vector3& right) const
	{
		return Vector3(
			_values[iX] - right.getX(),
			_values[iY] - right.getY(),
			_values[iZ] - right.getZ());
	}

	Vector3 Vector3::operator-(const Vector2& right) const
	{
		return Vector3(
			_values[iX] - right.getX(),
			_values[iY] - right.getY(),
			_values[iZ]);
	}

	Vector3 Vector3::operator*(const Vector3& right) const
	{
		return Vector3(
			_values[iX] * right.getX(),
			_values[iY] * right.getY(),
			_values[iZ] * right.getZ());
	}

	Vector3 Vector3::operator*(float right) const
	{
		return Vector3(
			_values[iX] * right,
			_values[iY] * right,
			_values[iZ] * right);
	}

	Vector3 Vector3::normalize() const
	{
		float l = length();
		return Vector3(getX() / l, getY() / l, getZ() / l);
	}

	Vector2 Vector3::toVector2() const
	{
		return Vector2(getX(), getY());
	}

	float Vector3::length() const
	{
		return Math::sqrt(lengthSqr());
	}

	float Vector3::lengthSqr() const
	{
		return getX()*getX() + getY()*getY() + getZ()*getZ();
	}

	Vector3& Vector3::operator/=(float v)
	{
		_values[0] /= v;
		_values[1] /= v;
		_values[2] /= v;
		return *this;
	}

	void Vector3::inverseY()
	{
		_values[iY] = -_values[iY];
	}

	void Vector3::inverseZ()
	{
		_values[iZ] = -_values[iZ];
	}

	bool Vector3::isEmpty() const
	{
		return Vector3::equals(*this, empty);
	}

	Vector3 Vector3::lerp(const Vector3& p1, const Vector3& p2, float u)
	{
		return Vector3(
			Math::lerp(p1.getX(), p2.getX(), u),
			Math::lerp(p1.getY(), p2.getY(), u),
			Math::lerp(p1.getZ(), p2.getZ(), u));
	}

	Vector3 Vector3::crossProduct(Vector3 u, Vector3 v)
	{
		return Vector3(
			u[iY] * v[iZ] - u[iZ] * v[iY],
			u[iZ] * v[iX] - u[iX] * v[iZ],
			u[iX] * v[iY] - u[iY] * v[iX]);
	}

	float Vector3::dotProduct(Vector3 v1, Vector3 v2)
	{
		return v1.getX()*v2.getX() + v1.getY()*v2.getY() + v1.getZ()*v2.getZ();
	}
}
