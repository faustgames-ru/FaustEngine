#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "graphics.h"
#include "RenderState.h"
#include "TextureRenderTarget2d.h"

namespace graphics
{
	class GraphicsDevice
	{
	public:
		static int poolSize(){ return 1; }
		RenderState renderState;
		GraphicsDevice();
		~GraphicsDevice();
		void setClearState(unsigned int color, float depth);
		void setViewport(int x, int y, int width, int height);
		void setRenderTarget(TextureRenderTarget2d *renderTarget);
		void clear();
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