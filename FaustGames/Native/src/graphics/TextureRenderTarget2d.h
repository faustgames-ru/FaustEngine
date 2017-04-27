#ifndef TEXTURE_RENDER_TARGET_2D_H
#define TEXTURE_RENDER_TARGET_2D_H

#include "graphics_classes.h"
#include "Texture.h"

namespace graphics
{
	class IRenderTarget
	{
	public:
		virtual GLuint getTextureHandle() = 0;
		virtual GLuint getFramebuffer() = 0;
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
	};

	class TextureRenderTarget2d : public Texture, public llge::IRenderTarget2d, public IRenderTarget
	{
	public:
		TextureRenderTarget2d();
		TextureRenderTarget2d(bool filter);
		virtual GLuint getTextureHandle() override { return getHandle(); }
		virtual int getWidth(){ return _width; }
		virtual int getHeight(){ return _height; }
		virtual GLuint getFramebuffer(){ return _frameBuffer; }
		inline GLuint getDepthbuffer(){ return _depthBuffer; }

		virtual void* API_CALL getRenderTargetInstance(){ return (IRenderTarget *)this; }
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

#endif /*TEXTURE_RENDER_TARGET_2D_H*/
