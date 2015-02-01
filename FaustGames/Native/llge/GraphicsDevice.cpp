#include "GraphicsDevice.h"
#include "Color.h"
#include "Errors.h"

namespace graphics
{
	GraphicsDevice::GraphicsDevice()
	{
	}
	
	GraphicsDevice::~GraphicsDevice()
	{
	}


	void GraphicsDevice::setClearState(unsigned int color, float depth)
	{
		glClearColor(
			(float)Color::getR(color) / 255.0f,
			(float)Color::getG(color) / 255.0f,
			(float)Color::getB(color) / 255.0f,
			(float)Color::getA(color) / 255.0f);
		glClearDepth(depth);
	}


	void GraphicsDevice::setViewPort(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void GraphicsDevice::setRenderTarget(RenderTarget *renderPath)
	{
	}

	void GraphicsDevice::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsDevice::drawPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
		renderState.apply(vertexFormat, vertexBuffer);
		glDrawElements(GL_TRIANGLES, primitivesCount * 3, GL_UNSIGNED_SHORT, indexBuffer);
		Errors::check(Errors::DrawElements);
	}

}
