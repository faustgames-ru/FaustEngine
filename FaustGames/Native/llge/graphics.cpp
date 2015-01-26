#include "stdafx.h"

namespace graphics
{
	unsigned char Color::getB(unsigned int value)
	{
		return (unsigned char)(value & 0xff);
	}
	
	unsigned char Color::getG(unsigned int value)	
	{
		return (unsigned char)((value >> 8) & 0xff);
	}
	
	unsigned char Color::getR(unsigned int value)
	{
		return (unsigned char)((value >> 16) & 0xff);
	}
	
	unsigned char Color::getA(unsigned int value)
	{
		return (unsigned char)(value >> 24);
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


	void GraphicsDevice::setViewPort(float x, float y, float width, float height)
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
	
	void GraphicsDevice::drawPrimitives(RenderState *renderState, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount)
	{
	}
}
