#ifndef DRAWING_CLASSES_H
#define DRAWING_CLASSES_H

#include "drawing_platform.h"
#include "drawing_references.h"

namespace drawing
{
	struct SolidVertex
	{
		float x;
		float y;
		float z;
		unsigned int color;
		SolidVertex()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			color = 0;
		}
		SolidVertex(const core::Vector3 &v, uint c)
		{
			x = v.getX();
			y = v.getY();
			z = v.getZ();
			color = c;
		}

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
		inline void setX(float value) { x = value; }
		inline void setY(float value) { y = value; }
		inline void setZ(float value) { z = value; }
	};

	struct Mesh2dVertex
	{
		float x;
		float y;
		float z;
		float u;
		float v;
		unsigned int color;

		Mesh2dVertex();
		Mesh2dVertex(
			float x,
			float y,
			float z,
			unsigned int color,
			float u,
			float v		
		);

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
		inline void setX(float value){ x = value; }
		inline void setY(float value){ y = value; }
		inline void setZ(float value){ z = value; }
	};

	inline Mesh2dVertex::Mesh2dVertex()
	{
	}

	inline Mesh2dVertex::Mesh2dVertex(float ix, float iy, float iz, unsigned int icolor, float iu, float iv)
	{
		x = ix;
		y = iy;
		z = iz;
		u = iu;
		v = iv;
		color = icolor;
	}

	class RendererTransform
	{
	public:
		core::Matrix *Matrix;

		template <typename T>
		void transform(T *target, T *source)
		{
			target->setX((source->x * Matrix->getXx() + source->y * Matrix->getYx() + source->z * Matrix->getZx() + Matrix->getWx()));
			target->setY((source->x * Matrix->getXy() + source->y * Matrix->getYy() + source->z * Matrix->getZy() + Matrix->getWy()));
			target->setZ((source->x * Matrix->getXz() + source->y * Matrix->getYz() + source->z * Matrix->getZz() + Matrix->getWz()));
		}
	};
}

#endif /*DRAWING_CLASSES_H*/

