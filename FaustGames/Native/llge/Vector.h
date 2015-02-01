#ifndef VECTOR_H
#define VECTOR_H

#include "stdafx.h"
#include "core.h"

namespace core
{
	class Vector2
	{
	private:
		float _values[2];
	public:
		inline const float *getData(){ return _values; }
		inline const float getX(){ return _values[0]; }
		inline const float getY(){ return _values[1]; }
	};

	class Vector3
	{
	private:
		float _values[3];
	public:
		Vector3();
		Vector3(float x, float y, float z)
		{
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
		}
		inline const float *getData(){ return _values; }
		inline const float getX(){ return _values[0]; }
		inline const float getY(){ return _values[1]; }
		inline const float getZ(){ return _values[2]; }
	};

	class Vector4
	{
	private:
		float _values[4];
	public:
		inline const float *getData(){ return _values; }
		inline const float getX(){ return _values[0]; }
		inline const float getY(){ return _values[1]; }
		inline const float getZ(){ return _values[2]; }
		inline const float getW(){ return _values[3]; }
	};
}

#endif /*VECTOR_H*/