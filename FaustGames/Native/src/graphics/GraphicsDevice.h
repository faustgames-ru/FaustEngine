#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "graphics_classes.h"
#include "RenderState.h"
#include "TextureRenderTarget2d.h"
#include "VertexBuffer.h"

namespace graphics
{
	class GraphicsDevice
	{
	public:
		static GraphicsDevice Default;
		RenderState renderState;
		GraphicsDevice();
		~GraphicsDevice();
		void setClearState(unsigned int color, float depth);
		void setViewport(int x, int y, int width, int height);
		void setRenderTarget(IRenderTarget *renderTarget);
		void clear();
		void clearDepth();
		void resetState();
		void drawVertexBuffer(VertexFormat *vertexFormat, GLuint vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
		void drawEdges(VertexFormat *vertexFormat, void *vertexBuffer, int primitivesCount);
		void drawTriangles(VertexFormat *vertexFormat, void *vertexBuffer, int primitivesCount);
		void drawPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
	protected:
	private:
		int _viewportX;
		int _viewportY;
		int _viewportWidth;
		int _viewportHeight;
	};
}

#endif /*GRAPHICSDEVICE_H*/