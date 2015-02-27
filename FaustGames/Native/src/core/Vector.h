#ifndef VECTOR_H
#define VECTOR_H

#include "core_classes.h"

namespace core
{
	class Vector2
	{
	private:
		float _values[2];
	public:
		Vector2(){}
		Vector2(float x, float y)
		{
			_values[0] = x;
			_values[1] = y;
		}
		inline const float *getData() const { return _values; }
		inline const float getX() const { return _values[0]; }
		inline const float getY() const { return _values[1]; }
		inline void setX(float value){ _values[0] = value; }
		inline void setY(float value){ _values[1] = value; }
	};

	class Vector3
	{
	private:
		float _values[3];
	public:
		Vector3() {}
		Vector3(float x, float y, float z)
		{
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
		}
		inline float *getData(){ return _values; }
		inline void setData(float x, float y, float z)
		{
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
		}
		inline float getX() const { return _values[0]; }
		inline float getY() const { return _values[1]; }
		inline float getZ() const { return _values[2]; }
		inline void setX(float value){ _values[0] = value; }
		inline void setY(float value){ _values[1] = value; }
		inline void setZ(float value){ _values[2] = value; }
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