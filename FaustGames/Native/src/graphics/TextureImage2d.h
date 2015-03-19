#ifndef TEXTURE_IMAGE_2D_H
#define TEXTURE_IMAGE_2D_H

#include "graphics_classes.h"
#include "TextureLoader.h"
#include "Texture.h"

namespace graphics
{
	class TextureImage2d : public Texture, public llge::ITextureImage2d
	{
	public:
		TextureImage2d();
		void setData(const Image2dData *data);
		void setData(int width, int height, Image2dFormat::e format, unsigned int *pixels);

		virtual ITexture* API_CALL getTexture(){ return this; }
		virtual void API_CALL LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels);
		virtual void API_CALL create();
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose(){ delete this; }
	protected:
	private:
		static GLenum getFormat(Image2dFormat::e format);
		bool _createMipmaps;
		bool _wrap;
		bool _filter;
	};
}

#endif /*TEXTURE_IMAGE_2D_H*/
