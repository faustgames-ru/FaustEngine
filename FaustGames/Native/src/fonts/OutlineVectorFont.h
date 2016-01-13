#ifndef FONTS_OUTLINE_VECTOR_FONT_H
#define FONTS_OUTLINE_VECTOR_FONT_H

#include "fonts_classes.h"
#include "FontCharSet.h"
#include "IFontRenderer.h"

namespace fonts
{
	class FontGlyphSegment
	{
	public:
		int p[4];
		FontGlyphSegment();
	};
	
	class FontGlyphOutline
	{
	public:
		std::vector<FontGlyphSegment> segments;
	};

	class FontGlyphVector
	{
	public:
		enum CurveTag
		{
			On,
			Conic,
			Cubic
		};
		core::Vector2 position;
		CurveTag tag;
		FontGlyphVector();
		FontGlyphVector(FT_Vector p, CurveTag t);
		FontGlyphVector(FT_Vector p0, FT_Vector p1, CurveTag t);
	};

	class OutlineVectorFontGlyph
	{
	public:
		std::vector<FontGlyphOutline> outlines;
		std::vector<core::Vector2> points;
		core::Vector2 advance;
		core::Vector2 min;
		core::Vector2 max;
		void addOutline(uint start, const std::vector<FontGlyphVector> &outlinePoints);
	};


	class OutlineVectorFont : public IBaseObject
	{
	public:
		OutlineVectorFont();
		~OutlineVectorFont();
		void load(void *buffer, int bytesCount, FontCharSet *charset);
		static void renderConic(const core::Vector3 &p1, const core::Vector3 &p2, const core::Vector3 &p3, IFontRenderer* renderer);
		static void renderCubic(const core::Vector3 &p1, const core::Vector3 &p2, const core::Vector3 &p3, const core::Vector3 &p4, IFontRenderer* renderer);
		core::Vector2 getSize(float scale, const char *text);
		void render(const core::Vector3 &position, float scale, const char *text, IFontRenderer* renderer);
	private:
		void loadOutline(FT_GlyphSlot slot, char symbol);
		static const int GlyphsCount = 256;
		OutlineVectorFontGlyph* _glyphs[GlyphsCount];
	};
}

#endif /*FONTS_OUTLINE_VECTOR_FONT_H*/
