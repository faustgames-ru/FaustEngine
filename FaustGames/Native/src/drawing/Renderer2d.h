#ifndef RENDERER_H
#define RENDERER_H

#include "drawing_classes.h"
#include "Camera.h"

namespace drawing
{
	struct Mesh2dVertex
	{
		float x;
		float y;
		float z;
		unsigned short u;
		unsigned short v;
		unsigned int color;

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
		inline void setX(float value){ x = value; }
		inline void setY(float value){ y = value; }
		inline void setZ(float value){ z = value; }
	};

	class RendererTransform2d
	{
	public:
		core::Vector3 LocalPivot;
		core::Vector3 GlobalPosition;
		float Sin;
		float Cos;
		float Scale;

		RendererTransform2d() :
			LocalPivot(0.0f, 0.0f, 0.0f),
			GlobalPosition(0.0f, 0.0f, 0.0f),
			Sin(0.0f),
			Cos(1.0f),
			Scale(1.0f)
		{
		}

		template <typename T>
		void transform(T *target, T *source)
		{
			float scaledX = (source->getX() - LocalPivot.getX()) * Scale;
			float scaledY = (source->getY() - LocalPivot.getY()) * Scale;
			float z = (source->getZ() - LocalPivot.getZ());

			float rotationX = scaledX * Cos - scaledY * Sin;
			float rotationY = scaledX * Sin + scaledY * Cos;

			target->setX(rotationX + GlobalPosition.getX());
			target->setY(rotationY + GlobalPosition.getY());
			target->setZ(z + GlobalPosition.getZ());
		}
	};

	class Renderer2d
	{
	public:
		RendererTransform2d Transform;
		Camera Camera;
		Renderer2d();
		void start();
		int finish();
		void drawMesh(unsigned int texture, Mesh2dVertex *vertices, int verticesCount, 
			unsigned short *indices, int indicesCount);
	private:
		void draw();

		static const int VerticesPerDraw = 32768;
		static const int IndicesPerDraw = 32768;
		unsigned int _texture;
		Mesh2dVertex* _vertices;
		unsigned short* _indices;
		int _verticesCount;
		int _indicesCount;

		graphics::GraphicsDevice * _graphicsDevice;
		graphics::VertexFormat * _format;
		core::MatrixContainer _projectionContainer;
		int _drawCalls;
	};
}

#endif /*RENDERER_H*/
