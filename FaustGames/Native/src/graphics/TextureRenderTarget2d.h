#ifndef TEXTURE_RENDER_TARGET_2D_H
#define TEXTURE_RENDER_TARGET_2D_H

#include "graphics_classes.h"
#include "Texture.h"

namespace graphics
{
	class TextureRenderTarget2d : public Texture, public llge::IRenderTarget2d
	{
	public:
		TextureRenderTarget2d();
		inline int getWidth(){ return _width; }
		inline int getHeight(){ return _height; }
		inline GLuint getFramebuffer(){ return _frameBuffer; }
		inline GLuint getDepthbuffer(){ return _depthBuffer; }

		virtual ITexture* API_CALL getTexture(){ return this; }
		virtual void API_CALL create(int width, int height);
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose() { delete this; }
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
