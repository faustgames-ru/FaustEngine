#ifndef VECTOR_H
#define VECTOR_H

#include "core_classes.h"
#include "Math.h"

namespace core
{
	class Vector3;

	struct Direction2
	{
		enum e
		{
			None = 0x0,
			X = 0x1,
			Y = 0x2,
			Left = 0x4 | X,
			Right = 0x8 | X,
			Top = 0x10 | Y,
			Bottom = 0x20 | Y,
		};
	};
	
	class Vector2
	{
	private:
		float _values[2];
	public:
		static Vector2 empty;
		static Vector2 axisX;
		static Vector2 axisY;
		static int iX;
		static int iY;

		Vector2();
		Vector2(float v);
		Vector2(float x, float y);
		Vector2& operator+=(const Vector2 &right);
		Vector2& operator+=(float right);
		Vector2& operator-=(const Vector2 &right);
		Vector2& operator*=(const Vector2 &right);
		Vector2& operator*=(float right);
		Vector2& operator/=(const Vector2 &right);
		Vector2& operator/=(float right);

		Vector2 operator+(const Vector2 &right) const;
		Vector2 operator-(const Vector2 &right) const;
		Vector2 operator*(float right) const;
		Vector2 operator*(const Vector2 &right) const;
		Vector2 operator/(float right) const;
		Vector2 operator/(const Vector2 &right) const;

		float operator[](int i) const;
		float &operator[](int i);

		const float* getData() const;
		float getX() const;
		float getY() const;
		void setX(float value);
		void setY(float value);
		void inverseY();
		void inverseX();
		Vector2 rotate90cw() const;
		Vector2 rotate90ccw() const;
		Vector2 inverse() const;
		Vector2 rotate(float a) const;
		Vector2 rotate(float c, float s) const;
		Vector2 normalize() const;
		Vector2 normalize(float eps) const;
		float length() const;
		float lengthSqr() const;
		Vector3 toVector3() const;
		Vector3 toVector3(float z) const;
		bool isEmpty() const;
		bool isDirectionX() const;
		Direction2::e direction() const;
		bool isEmpty(float eps) const;
		static Vector2 lerp(const Vector2 &from, const Vector2 &to, float u);
		static Vector2 cubic(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float u);
		static float crossProduct(Vector2 v1, Vector2 v2);
		static float dotProduct(Vector2 v1, Vector2 v2);
		static bool equals(const Vector2& a, const Vector2& b);
		static bool equals(const Vector2& a, const Vector2& b, float eps);
		static float distanceToEdge(const Vector2 &p, const Vector2 &e0, const Vector2 &e1);
		bool isInsideTriangle(const Vector2& a, const Vector2& b, const Vector2& c) const;
	};

	class Vector3
	{
	private:
		float _values[3];
	public:
		static Vector3 infinity;
		static Vector3 eX;
		static Vector3 eY;
		static Vector3 eZ;
		static int iX;
		static int iY;
		static int iZ;
		static Vector3 empty;
		Vector3() {}
		Vector3(float v);
		Vector3(float x, float y, float z)
		{
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
		}
		inline float *getData(){ return _values; }
		inline const float *getData() const { return _values; }
		inline void setData(float x, float y, float z)
		{
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
		}
		inline void setXy(float x, float y)
		{
			_values[0] = x;
			_values[1] = y;
		}
		inline void setXy(core::Vector2 xy)
		{
			_values[0] = xy.getX();
			_values[1] = xy.getY();
		}
		inline float getX() const { return _values[0]; }
		inline float getY() const { return _values[1]; }
		inline float getZ() const { return _values[2]; }
		inline void setX(float value){ _values[0] = value; }
		inline void setY(float value){ _values[1] = value; }
		inline void setZ(float value){ _values[2] = value; }
		
		float operator[](int i) const;
		float &operator[](int i);
		
		Vector3& operator+=(const Vector2& right);
		Vector3& operator-=(const Vector2& right);
		Vector3& operator*=(float right);
		Vector3 operator+(const Vector3 &right) const;
		Vector3 operator+(const Vector2 &right) const;
		Vector3 operator-(const Vector3 &right) const;
		Vector3 operator-(const Vector2 &right) const;
		Vector3 operator*(const Vector3 &right) const;
		Vector3 operator*(float right) const;
		Vector3 normalize() const;
		Vector2 toVector2() const;
		float length() const;
		float lengthSqr() const;
		Vector3& operator/=(float v);
		void inverseY();
		void inverseZ();
		bool isEmpty() const;
		static Vector3 lerp(const Vector3 &p1, const Vector3 &p2, float u);
		static Vector3 crossProduct(Vector3 v1, Vector3 v2);
		static float dotProduct(Vector3 v1, Vector3 v2);
		static inline bool equals(const Vector3 &a, const Vector3 &b)
		{
			return
				core::Math::equals(a.getX(), b.getX()) &&
				core::Math::equals(a.getY(), b.getY()) &&
				core::Math::equals(a.getZ(), b.getZ());
		}
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

	struct MeshVertex
	{
		Vector3 xyz;
		uint color;
		Vector2 uv;
		MeshVertex();
		MeshVertex(Vector2 xy);
		MeshVertex(Vector3 xyz);
		MeshVertex(Vector2 xy, Vector2 uv);
		MeshVertex(Vector2 xy, float u, float v);
		MeshVertex(Vector3 xyz, Vector2 uv);
		MeshVertex(Vector2 xyz, Vector2 uv, uint color);
		MeshVertex(Vector3 xyz, Vector2 uv, uint color);
		MeshVertex(float x, float y, float z, float u, float v, uint color);
		MeshVertex(float x, float y, float u, float v, uint color);
	};

	class MeshIndexer
	{
	public:
		int Type;
		int VerticesFirst;
		int VerticesCount;
		int IndicesFirst;
		int IndicesCount;
	};
}

#endif /*VECTOR_H*/