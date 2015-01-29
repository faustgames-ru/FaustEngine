#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "graphics.h"

namespace graphics
{
	class GraphicsDevice
	{
	public:
		static int poolSize(){ return 1; }
		void setClearState(unsigned int color, float depth);
		void setViewPort(int x, int y, int width, int height);
		void setRenderTarget(RenderTarget *renderPath);
		void clear();
		void drawPrimitives(RenderState *renderState, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
	};
}

#endif /*GRAPHICSDEVICE_H*/