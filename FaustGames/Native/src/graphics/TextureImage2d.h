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
		static int Size;
		static TextureImage2d * empty(){ return &_empty; }

		TextureImage2d(bool generateMipmaps, bool useFilter);
		void setData(const Image2dData *data);
		void setData(int width, int height, Image2dFormat::e format, unsigned int *pixels);
		static void createStatic();
		static void cleanupStatic();

		virtual ITexture* API_CALL getTexture(){ return this; }
		virtual void API_CALL LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels);
		virtual void API_CALL create();
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose(){ delete this; }
	protected:
	private:
		TextureImage2d();
		static GLenum getFormat(Image2dFormat::e format);
		bool _createMipmaps;
		bool _wrap;
		bool _filter;
		int _size;
		static TextureImage2d _empty;
	};
}

#endif /*TEXTURE_IMAGE_2D_H*/
