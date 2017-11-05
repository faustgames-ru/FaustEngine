#ifndef TEXTURE_RENDER_TARGET_DEPTH_2D_H
#define TEXTURE_RENDER_TARGET_DEPTH_2D_H

#include "graphics_classes.h"
#include "Texture.h"
#include "TextureRenderTarget2d.h"
namespace graphics
{
	class TextureRenderTargetDepth2d : public llge::IRenderTargetDepth2d, public IRenderTarget
	{
	public:
		TextureRenderTargetDepth2d();
		virtual int getWidth(){ return _width; }
		virtual int getHeight(){ return _height; }
		virtual GLuint getFramebuffer(){ return _frameBuffer; }
		virtual GLuint getTextureHandle() override { return _colorTexture.getHandle(); }

		virtual void* API_CALL getRenderTargetInstance(){ return (IRenderTarget *)this; }
		virtual llge::ITexture* API_CALL getTexture(){ return &_colorTexture; }
		virtual llge::ITexture* API_CALL getDepthTexture(){ return &_depthTexture; }
		virtual void API_CALL create(int width, int height);
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose() { delete this; }
		Texture _colorTexture;
		Texture _depthTexture;
	protected:
	private:
		bool _filter;
		int _width;
		int _height;
		GLuint _frameBuffer;
	};
}

#endif /*TEXTURE_RENDER_TARGET_DEPTH_2D_H*/
