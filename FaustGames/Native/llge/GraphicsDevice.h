#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "graphics.h"

namespace graphics
{
	class GraphicsDevice
	{
	public:
		void setClearState(unsigned int color, float depth);
		void setViewPort(float x, float y, float width, float height);
		void setRenderTarget(RenderTarget *renderPath);
		void clear();
		void drawPrimitives(RenderState *renderState, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
	};
}

#endif /*GRAPHICSDEVICE_H*/