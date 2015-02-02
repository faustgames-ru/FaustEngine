#ifndef TEXTURE_RENDER_TARGET_2D_H
#define TEXTURE_RENDER_TARGET_2D_H

#include "graphics.h"
#include "Texture.h"

namespace graphics
{
	class TextureRenderTarget2d : public Texture
	{
	public:
		TextureRenderTarget2d();
		void create(int width, int height);
		void cleanup();
		inline int getWidth(){ return _width; }
		inline int getHeight(){ return _height; }
		inline GLuint getFramebuffer(){ return _frameBuffer; }
		inline GLuint getDepthbuffer(){ return _depthBuffer; }
	protected:
	private:
		bool _filter;
		int _width;
		int _height;
		GLuint _frameBuffer;
		GLuint _depthBuffer;
	};
}

#endif /*TEXTURE_RENDER_TARGET2D_H*/
