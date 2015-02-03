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
		inline const float *getData(){ return _values; }
		static Matrix identity;
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
	};
}

#endif /*MATRIX_H*/