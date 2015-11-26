#ifndef MATRIX_H
#define MATRIX_H

#include "core_classes.h"

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
		static float DotProduct(
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2)
		{
			return x1*x2 + y1*y2 + z1*z2 + w1*w2;
		}
		static Matrix mul(Matrix a, Matrix b)
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

		inline float getXx() { return _values[0]; }
		inline float getXy() { return _values[1]; }
		inline float getXz() { return _values[2]; }
		inline float getXw() { return _values[3]; }

		inline float getYx() { return _values[4]; }
		inline float getYy() { return _values[5]; }
		inline float getYz() { return _values[6]; }
		inline float getYw() { return _values[7]; }

		inline float getZx() { return _values[8]; }
		inline float getZy() { return _values[9]; }
		inline float getZz() { return _values[10]; }
		inline float getZw() { return _values[11]; }

		inline float getWx() { return _values[12]; }
		inline float getWy() { return _values[13]; }
		inline float getWz() { return _values[14]; }
		inline float getWw() { return _values[15]; }

		static Matrix identity;

		static Matrix createTranslate(float x, float y, float z)
		{
			return Matrix(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				x, y, z, 1);
		}

		static Matrix createProjection(float fov, float aspect, float zn, float zf)
		{
			float h = 1.0f / tan(fov *0.5f);
			float xScale = h * aspect;
			float yScale = h;

			return core::Matrix(
				xScale, 0, 0, 0,
				0, yScale, 0, 0,
				0, 0, zf / (zf - zn), 1,
				0, 0, -zn * zf / (zf - zn), 0);
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