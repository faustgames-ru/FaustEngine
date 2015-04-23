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
		float u;
		float v;
		unsigned int color;

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
		inline void setX(float value){ x = value; }
		inline void setY(float value){ y = value; }
		inline void setZ(float value){ z = value; }
	};

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

	class Renderer2d : public llge::IBatch2d
	{
	public:
		RendererTransform Transform;
		Camera RendererCamera;
		Renderer2d();
		void start();
		int finish();
		void drawMesh(unsigned int texture, Mesh2dVertex *vertices, int verticesCount,
			unsigned short *indices, int indicesCount);

		virtual void API_CALL setTransform(void *transform){ RendererCamera.setMatrix((float *)transform); }
		virtual void API_CALL startBatch() { start(); }
		virtual void API_CALL finishBatch() { finish(); }
		virtual void API_CALL setTexture(llge::ITexture *texture) { _textureId = texture->getId(); }
		virtual void API_CALL draw(void *vertices, int verticesCount, void *indices, int indicesCount) 
		{
			drawMesh(_textureId, (Mesh2dVertex *)vertices, verticesCount, (unsigned short *)indices, indicesCount);
		}

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
		unsigned int _textureId;
	};
}

#endif /*RENDERER_H*/
