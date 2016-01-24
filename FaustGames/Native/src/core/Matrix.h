#ifndef MATRIX_H
#define MATRIX_H

#include "core_classes.h"
#include "Math.h"
#include "Vector.h"

namespace core
{
	class Matrix
	{
	private:
		float _values[16];
	public:
		Matrix() {}
		inline Matrix(
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44) 
		{
			_values[0] = m11; 
			_values[1] = m12; 
			_values[2] = m13; 
			_values[3] = m14;

			_values[4] = m21; 
			_values[5] = m22; 
			_values[6] = m23; 
			_values[7] = m24;

			_values[8] = m31; 
			_values[9] = m32; 
			_values[10] = m33; 
			_values[11] = m34;

			_values[12] = m41; 
			_values[13] = m42; 
			_values[14] = m43; 
			_values[15] = m44;
		}
		inline void setData(const float *value)
		{
			for (int i = 0; i < 16; i++)
				_values[i] = value[i];
		}
		
		inline const float *getData() const { return &(_values[0]); }

		Matrix toSkyboxProjection() const;

		static bool equals(const Matrix &a, const Matrix &b);
		static Matrix createScale(float x, float y, float z);
		static Matrix createRotation(Vector3 n, float a);

		static float DotProduct(
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2)
		{
			return x1*x2 + y1*y2 + z1*z2 + w1*w2;
		}
		static Matrix mul(const Matrix &a, const Matrix &b, const Matrix &c)
		{
			return mul(mul(a, b), c);
		}
		static Matrix mul(const Matrix &a, const Matrix &b, const Matrix &c, const Matrix &d)
		{
			return mul(mul(mul(a, b), c), d);
		}
		static Matrix mul(const Matrix &a, const Matrix &b, const Matrix &c, const Matrix &d, const Matrix &e)
		{
			return mul(mul(mul(mul(a, b), c), d), e);
		}
		static Matrix mul(const Matrix &a, const Matrix &b)
		{
			return Matrix(
				DotProduct(a.getXx(), a.getXy(), a.getXz(), a.getXw(), b.getXx(), b.getYx(), b.getZx(), b.getWx()),
				DotProduct(a.getXx(), a.getXy(), a.getXz(), a.getXw(), b.getXy(), b.getYy(), b.getZy(), b.getWy()),
				DotProduct(a.getXx(), a.getXy(), a.getXz(), a.getXw(), b.getXz(), b.getYz(), b.getZz(), b.getWz()),
				DotProduct(a.getXx(), a.getXy(), a.getXz(), a.getXw(), b.getXw(), b.getYw(), b.getZw(), b.getWw()),
				DotProduct(a.getYx(), a.getYy(), a.getYz(), a.getYw(), b.getXx(), b.getYx(), b.getZx(), b.getWx()),
				DotProduct(a.getYx(), a.getYy(), a.getYz(), a.getYw(), b.getXy(), b.getYy(), b.getZy(), b.getWy()),
				DotProduct(a.getYx(), a.getYy(), a.getYz(), a.getYw(), b.getXz(), b.getYz(), b.getZz(), b.getWz()),
				DotProduct(a.getYx(), a.getYy(), a.getYz(), a.getYw(), b.getXw(), b.getYw(), b.getZw(), b.getWw()),
				DotProduct(a.getZx(), a.getZy(), a.getZz(), a.getZw(), b.getXx(), b.getYx(), b.getZx(), b.getWx()),
				DotProduct(a.getZx(), a.getZy(), a.getZz(), a.getZw(), b.getXy(), b.getYy(), b.getZy(), b.getWy()),
				DotProduct(a.getZx(), a.getZy(), a.getZz(), a.getZw(), b.getXz(), b.getYz(), b.getZz(), b.getWz()),
				DotProduct(a.getZx(), a.getZy(), a.getZz(), a.getZw(), b.getXw(), b.getYw(), b.getZw(), b.getWw()),
				DotProduct(a.getWx(), a.getWy(), a.getWz(), a.getWw(), b.getXx(), b.getYx(), b.getZx(), b.getWx()),
				DotProduct(a.getWx(), a.getWy(), a.getWz(), a.getWw(), b.getXy(), b.getYy(), b.getZy(), b.getWy()),
				DotProduct(a.getWx(), a.getWy(), a.getWz(), a.getWw(), b.getXz(), b.getYz(), b.getZz(), b.getWz()),
				DotProduct(a.getWx(), a.getWy(), a.getWz(), a.getWw(), b.getXw(), b.getYw(), b.getZw(), b.getWw()));
		}

		inline float getXx() const { return _values[0]; }
		inline float getXy() const { return _values[1]; }
		inline float getXz() const { return _values[2]; }
		inline float getXw() const { return _values[3]; }

		inline float getYx() const { return _values[4]; }
		inline float getYy() const { return _values[5]; }
		inline float getYz() const { return _values[6]; }
		inline float getYw() const { return _values[7]; }

		inline float getZx() const { return _values[8]; }
		inline float getZy() const { return _values[9]; }
		inline float getZz() const { return _values[10]; }
		inline float getZw() const { return _values[11]; }

		inline float getWx() const { return _values[12]; }
		inline float getWy() const { return _values[13]; }
		inline float getWz() const { return _values[14]; }
		inline float getWw() const { return _values[15]; }

		Matrix transpose() const
		{
			return Matrix(
				getXx(), getYx(), getZx(), getWx(),
				getXy(), getYy(), getZy(), getWy(),
				getXz(), getYz(), getZz(), getWz(),
				getXw(), getYw(), getZw(), getWw());
		}

		Matrix inverse()
		{
			float xx = +determinant3X3(getYy(), getYz(), getYw(), getZy(), getZz(), getZw(), getWy(), getWz(), getWw());
			float xy = -determinant3X3(getYx(), getYz(), getYw(), getZx(), getZz(), getZw(), getWx(), getWz(), getWw());
			float xz = +determinant3X3(getYx(), getYy(), getYw(), getZx(), getZy(), getZw(), getWx(), getWy(), getWw());
			float xw = -determinant3X3(getYx(), getYy(), getYz(), getZx(), getZy(), getZz(), getWx(), getWy(), getWz());

			float yx = -determinant3X3(getXy(), getXz(), getXw(), getZy(), getZz(), getZw(), getWy(), getWz(), getWw());
			float yy = +determinant3X3(getXx(), getXz(), getXw(), getZx(), getZz(), getZw(), getWx(), getWz(), getWw());
			float yz = -determinant3X3(getXx(), getXy(), getXw(), getZx(), getZy(), getZw(), getWx(), getWy(), getWw());
			float yw = +determinant3X3(getXx(), getXy(), getXz(), getZx(), getZy(), getZz(), getWx(), getWy(), getWz());

			float zx = +determinant3X3(getXy(), getXz(), getXw(), getYy(), getYz(), getYw(), getWy(), getWz(), getWw());
			float zy = -determinant3X3(getXx(), getXz(), getXw(), getYx(), getYz(), getYw(), getWx(), getWz(), getWw());
			float zz = +determinant3X3(getXx(), getXy(), getXw(), getYx(), getYy(), getYw(), getWx(), getWy(), getWw());
			float zw = -determinant3X3(getXx(), getXy(), getXz(), getYx(), getYy(), getYz(), getWx(), getWy(), getWz());

			float wx = -determinant3X3(getXy(), getXz(), getXw(), getYy(), getYz(), getYw(), getZy(), getZz(), getZw());
			float wy = +determinant3X3(getXx(), getXz(), getXw(), getYx(), getYz(), getYw(), getZx(), getZz(), getZw());
			float wz = -determinant3X3(getXx(), getXy(), getXw(), getYx(), getYy(), getYw(), getZx(), getZy(), getZw());
			float ww = +determinant3X3(getXx(), getXy(), getXz(), getYx(), getYy(), getYz(), getZx(), getZy(), getZz());

			float l = getXx()*xx + getXy()*xy + getXz()*xz + getXw()*xw;
			if (Math::abs(l) < Math::Epsilon)
			{
				return *this;
			}
			float d = 1 / l;
			return Matrix(
				xx*d, yx*d, zx*d, wx*d,
				xy*d, yy*d, zy*d, wy*d,
				xz*d, yz*d, zz*d, wz*d,
				xw*d, yw*d, zw*d, ww*d);
		}

		inline void getRotation(float& p, float& h, float& b) const
		{
			b = Math::atan2(getYx(), getYy());
			h = Math::atan2(getXz(), getZz());
			float sinP = -getYz();
			if (sinP > 1.0f)
				sinP = 1.0f;
			if (sinP < -1.0f)
				sinP = -1.0f;
			p = Math::asin(sinP);
		}

		static Matrix identity;

		static Matrix createEuler(float p, float h, float b)
		{
			return Matrix(
				Math::cos(h)*Math::cos(b) + Math::sin(h)*Math::sin(p)*Math::sin(b), 
				Math::cos(b)*Math::sin(h)*Math::sin(p) - Math::sin(b)*Math::cos(h), 
				Math::cos(p)*Math::sin(h), 0,
				Math::cos(p)*Math::sin(b), 
				Math::cos(b)*Math::cos(p), 
				-Math::sin(p), 0,
				Math::sin(b)*Math::cos(h)*Math::sin(p) - Math::sin(h)*Math::cos(b), 
				Math::sin(h)*Math::sin(b) + Math::cos(b)*Math::cos(h)*Math::sin(p), 
				Math::cos(p)*Math::cos(h), 0, 0, 0, 0, 1);

		}
			static Vector3 transform(Matrix m, Vector2 v)
		{
			float w = 1.0f / (v.getX() * m.getXw() + v.getY() * m.getYw() + m.getWw());
			return Vector3(
				(v.getX() * m.getXx() + v.getY() * m.getYx() + m.getWx()) * w,
				(v.getX() * m.getXy() + v.getY() * m.getYy() + m.getWy()) * w,
				(v.getX() * m.getXz() + v.getY() * m.getYz() + m.getWz()) * w);
		}
		
		static Vector3 transform(Matrix m, Vector3 v)
		{
			float w = 1.0f / (v.getX() * m.getXw() + v.getY() * m.getYw() + v.getZ() * m.getZw() + m.getWw());
			return Vector3(
				(v.getX() * m.getXx() + v.getY() * m.getYx() + v.getZ() * m.getZx() + m.getWx()) * w,
				(v.getX() * m.getXy() + v.getY() * m.getYy() + v.getZ() * m.getZy() + m.getWy()) * w,
				(v.getX() * m.getXz() + v.getY() * m.getYz() + v.getZ() * m.getZz() + m.getWz()) * w);
		}

		static Matrix createTranslate(float x, float y, float z)
		{
			return Matrix(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				x, y, z, 1);
		}
		
		static Matrix createOrtho(float aspect, float scale, float depth)
		{
			float h = scale;
			float w = h * aspect;

			return Matrix(
				1.0f / w, 0, 0, 0,
				0, 1.0f / h, 0, 0,
				0, 0, 1.0f / depth, 0.0f,
				0, 0, 0, 1);
		}
		
		static Matrix createProjection(float fov, float aspect, float zn, float zf)
		{
			float h = 1.0f / tan(fov *0.5f);
			float xScale = h / aspect;
			float yScale = h;

			return Matrix(
				xScale, 0, 0, 0,
				0, yScale, 0, 0,
				0, 0, zf / (zf - zn), 1,
				0, 0, -zn * zf / (zf - zn), 0);
		}

		static float determinant3X3(
			float xx, float xy, float xz,
			float yx, float yy, float yz,
			float zx, float zy, float zz)
		{
			return xx * yy * zz + xy * yz * zx + yx * zy * xz - zx * yy * xz - xy * yx * zz - zy * yz * xx;
		}


	};

	class MatrixContainer
	{
	private:
		static int _id;
	public:
		int id;
		Matrix Value;
		inline MatrixContainer()
		{
			id = (++_id) << 16;
		}
		
		inline MatrixContainer(const Matrix &value)
		{
			id = (++_id) << 16;
			Value = value;
		}

		inline void setValue(const Matrix &value)
		{
			++id;
			Value = value;
		}
		
		inline void setValue(const float *value)
		{
			++id;
			Value = *((Matrix *)value);
		}
	};

	class Matrix2
	{
	public:
		Matrix2(float m11, float m12, float m21, float m22);		
		inline float getXx() const;
		inline float getXy() const;
		inline float getYx() const;
		inline float getYy() const;
		static Matrix2 identity;		
		inline static Matrix2 mul(const Matrix2 &a, const Matrix2 &b);
		inline static Matrix2 createRotation(float angle);
		inline static Matrix2 createScale(float scaleX, float scaleY);
	private:
		float _values[4];
	};
}

#endif /*MATRIX_H*/