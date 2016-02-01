#ifndef FONTS_BITMAP_FONT_H
#define FONTS_BITMAP_FONT_H

#include "fonts_classes.h"
#include "FontCharSet.h"
#include "../graphics/TextureImage2d.h"

namespace fonts
{
	class BitmapFontVertex
	{
	public:
		core::Vector2 xy;
		unsigned short u;
		unsigned short v;
	};

	class IBitmapFontRenderer : IBaseObject
	{
	public:
		virtual void drawQuad(uint color, graphics::Texture *texture, const BitmapFontVertex &min, const BitmapFontVertex &max, float z, bool rotate) = 0;
	};

	class BitmapFontGlyph
	{
	public:
		int pageIndex;
		BitmapFontVertex min;
		BitmapFontVertex max;
		core::Vector2 advance;
		bool rotation;
	};
	class BitmapFont : public IBaseObject
	{
	public:
		float fontPixelSize;
		BitmapFont();
		~BitmapFont();
		core::Vector2 getSize(float scale, const char* text);
		void render(int x, int y, uint color, float scale, const char *text, IBitmapFontRenderer* renderer);
		void render(int x, int y, uint color, const char *text, IBitmapFontRenderer* renderer);
		void render(const core::Vector3 &position, float scale, uint color, const char *text, IBitmapFontRenderer* renderer);
		void load(const void *buffer, int bytesCount, float pixelSize, FontCharSet *charset);
		void unload();
		std::vector<graphics::TextureImage2d *> _textures;
	private:
		static const int GlyphsCount = 256;
		BitmapFontGlyph* _glyphs[GlyphsCount];
	};
}

#endif /*FONTS_BITMAP_FONT_H*/
