#include "OutlineVectorFont.h"
#include "FontsManager.h"
#include <freetype/ftbbox.h>


namespace fonts
{
	FontGlyphSegment::FontGlyphSegment()
	{
		p[0] = -1;	
		p[1] = -1;
		p[2] = -1;
		p[3] = -1;
	}

	FontGlyphVector::FontGlyphVector(): tag(On)
	{
	}

	FontGlyphVector::FontGlyphVector(FT_Vector p, CurveTag t)
	{
		position = core::Vector2(static_cast<float>(p.x) / 64.0f, static_cast<float>(p.y) / 64.0f);
		tag = t;
	}

	FontGlyphVector::FontGlyphVector(FT_Vector p0, FT_Vector p1, CurveTag t)
	{
		position = core::Vector2(
			static_cast<float>(p0.x + p1.x) / 128.0f, 
			static_cast<float>(p0.y + p1.y) / 128.0f);
		tag = t;
	}

	void OutlineVectorFontGlyph::addOutline(uint start, const std::vector<FontGlyphVector>& outlinePoints)
	{
		outlines.push_back(FontGlyphOutline());
		FontGlyphSegment s;
		uint si = 0;
		uint first = points.size();
		for (uint i = start; i < outlinePoints.size(); i++)
		{
			if (outlinePoints[i].tag == FontGlyphVector::On)
			{
				if (si > 0)
				{
					s.p[si] = points.size();
					outlines.back().segments.push_back(s);
					s = FontGlyphSegment();
					si=0;
				}
			}
			s.p[si] = points.size();
			si++;
			if (si > 3)
			{
				outlines.back().segments.push_back(s);
				s = FontGlyphSegment();
				si = 0;

				// error
			}
			points.push_back(outlinePoints[i].position);
		}
		s.p[si] = first;
		outlines.back().segments.push_back(s);
	}

	OutlineVectorFont::OutlineVectorFont()
	{
		for (uint i = 0; i < GlyphsCount; i++)
		{
			_glyphs[i] = nullptr;
		}
	}

	OutlineVectorFont::~OutlineVectorFont()
	{
		for (uint i = 0; i < GlyphsCount; i++)
		{
			if (_glyphs[i] != nullptr)
			{
				delete _glyphs[i];
			}
		}
	}

	void OutlineVectorFont::load(void* buffer, int bytesCount, FontCharSet* charset)
	{
		FT_Library library = static_cast<FT_Library>(FontsManager::Default.getFontLibraryInstance());
		FT_Byte* byteBuffer = static_cast<FT_Byte*>(buffer);
		FT_Face face;
		FT_Error error = FT_New_Memory_Face(library,
			byteBuffer,    /* first byte in memory */
			bytesCount,      /* size in bytes        */
			0,         /* face_index           */
			&face);
		if (error)
		{
			FontsManager::Default.onFontError(error);
			return;
		}

		FT_GlyphSlot slot = face->glyph;
		error = FT_Set_Char_Size(
			face,    /* handle to face object           */
			0,       /* char_width in 1/64th of points  */
			512 * 64,   /* char_height in 1/64th of points */
			72,     /* horizontal device resolution    */
			72);   /* vertical device resolution      */

		for (int n = 0; n < charset->charset.size(); n++)
		{
			/* retrieve glyph index from character code */
			char symbol = charset->charset[n];
			FT_UInt glyph_index = FT_Get_Char_Index(face, symbol);
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error)
			{
				continue;  /* ignore errors */
			}

			switch (slot->format)
			{				
			case FT_GLYPH_FORMAT_NONE:
				break;
			case FT_GLYPH_FORMAT_BITMAP:
				break;
			case FT_GLYPH_FORMAT_PLOTTER:
				break;
			case FT_GLYPH_FORMAT_OUTLINE:
				loadOutline(slot, symbol);
				break;
			case FT_GLYPH_FORMAT_COMPOSITE:
				break;
			}
		}

		FT_Done_Face(face);
	}

	void OutlineVectorFont::renderConic(const core::Vector3 &p1, const core::Vector3 &p2, const core::Vector3 &p3, IFontRenderer* renderer)
	{
		core::Vector3 from = p1;
		float step = 0.25f;
		float h = 1.0f + step * 0.5f;
		for (float t = step; t < h; t += step)
		{
			core::Vector3 a = core::Vector3::lerp(p1, p2, t);
			core::Vector3 b = core::Vector3::lerp(p2, p3, t);
			core::Vector3 p = core::Vector3::lerp(a, b, t);
			renderer->drawEdge(0xffffffff, from, p);
			from = p;
		}
	}

	void OutlineVectorFont::renderCubic(const core::Vector3& p1, const core::Vector3& p2, const core::Vector3& p3, const core::Vector3& p4, IFontRenderer* renderer)
	{
		core::Vector3 from = p1;
		float step = 0.2f;
		float h = 1.0f + step * 0.5f;
		for (float t = step; t < h; t += step)
		{
			core::Vector3 a = core::Vector3::lerp(p1, p2, t);
			core::Vector3 b = core::Vector3::lerp(p2, p3, t);
			core::Vector3 c = core::Vector3::lerp(p3, p4, t);

			core::Vector3 d = core::Vector3::lerp(a, b, t);
			core::Vector3 e = core::Vector3::lerp(b, c, t);

			core::Vector3 p = core::Vector3::lerp(d, e, t);
			renderer->drawEdge(0xffffffff, from, p);
			from = p;
		}
	}

	core::Vector2 OutlineVectorFont::getSize(float scale, const char* text)
	{
		scale /= 512.0f;
		core::Vector2 pen = core::Vector2::empty;
		geometry::Aabb2d aabb(core::Vector2::empty, core::Vector2::empty);
		for (uint i = 0; text[i] != 0; i++)
		{
			OutlineVectorFontGlyph* glyph = _glyphs[text[i]];
			if (glyph != nullptr)
			{				
				aabb.expandY(pen.getY() + glyph->min.getY() * scale);
				aabb.expandY(pen.getY() + glyph->max.getY() * scale);

				pen.setX(pen.getX() + glyph->advance.getX() * scale);
				
				aabb.expand(pen.getX(), pen.getY());
				aabb.expandX(pen.getX());
			}
		}
		return aabb.Max - aabb.Min;
	}

	void OutlineVectorFont::render(const core::Vector3 &position, float scale, const char* text, IFontRenderer* renderer)
	{
		if (text == nullptr) return;
		core::Vector3 pen = position;
		scale /= 512.0f;
		for (uint i = 0; text[i] != 0; i++)
		{
			FontGlyphSegment seg;
			OutlineVectorFontGlyph* glyph = _glyphs[text[i]];
			if (glyph != nullptr)
			{
				for (uint j = 0; j < glyph->outlines.size(); j++)
				{
					for (uint k = 0; k < glyph->outlines[j].segments.size(); k++)
					{
						seg = glyph->outlines[j].segments[k];
						if (seg.p[3] >= 0)
						{
							// cubic
							renderCubic(
								pen + (glyph->points[seg.p[0]] * scale).toVector3(),
								pen + (glyph->points[seg.p[1]] * scale).toVector3(),
								pen + (glyph->points[seg.p[2]] * scale).toVector3(), 
								pen + (glyph->points[seg.p[3]] * scale).toVector3(), renderer);
						}
						else if (seg.p[2] >= 0) 
						{
							// conic
							renderConic(
								pen + (glyph->points[seg.p[0]] * scale).toVector3(),
								pen + (glyph->points[seg.p[1]] * scale).toVector3(),
								pen + (glyph->points[seg.p[2]] * scale).toVector3(), renderer);
						}
						else
						{
							// line
							renderer->drawEdge(0xffffffff,
								pen + (glyph->points[seg.p[0]] * scale).toVector3(),
								pen + (glyph->points[seg.p[1]] * scale).toVector3());
						}
					}
				}
				pen.setX(pen.getX() + glyph->advance.getX() * scale);
			}
		}
	}

	void OutlineVectorFont::loadOutline(FT_GlyphSlot slot, char symbol)
	{
		if (_glyphs[symbol] != nullptr) return;
		OutlineVectorFontGlyph *glyph = new OutlineVectorFontGlyph();
		glyph->advance = core::Vector2(static_cast<float>(slot->advance.x) / 64.0f, static_cast<float>(slot->advance.y) / 64.0f);
		glyph->min = core::Vector2(static_cast<float>(slot->metrics.horiBearingX) / 64.0f, static_cast<float>(slot->metrics.horiBearingY - slot->metrics.height) / 64.0f);
		glyph->max = core::Vector2(static_cast<float>(slot->metrics.horiBearingX + slot->metrics.width) / 64.0f, static_cast<float>(slot->metrics.horiBearingY) / 64.0f);
		_glyphs[symbol] = glyph;
		uint contoursCount = slot->outline.n_contours;
		uint start = 0;
		std::vector<FontGlyphVector> contour;

		FT_Vector p;
		FT_Vector prev;
		char prevTag;
		short end;

		for (uint i = 0; i < contoursCount; i++)
		{
			end = slot->outline.contours[i];
			contour.clear();	
			int onCount = 0;
			for (uint j = start; j <= end; j++)
			{
				if (j < slot->outline.n_points)
				{
					p = slot->outline.points[j];
					char tag = FT_CURVE_TAG(slot->outline.tags[j]);

					switch (tag)
					{
					case FT_CURVE_TAG_ON:
						contour.push_back(FontGlyphVector(p, FontGlyphVector::On));
						onCount++;
						break;
					case FT_CURVE_TAG_CONIC:
						if (j == start)
						{
							prev = slot->outline.points[end];
							prevTag = FT_CURVE_TAG(slot->outline.tags[end]);
							if (prevTag == FT_CURVE_TAG_CONIC)
							{
								contour.push_back(FontGlyphVector(p, prev, FontGlyphVector::On));
								onCount++;
							}
						}
						else
						{
							prev = slot->outline.points[j-1];
							prevTag = FT_CURVE_TAG(slot->outline.tags[j-1]);
							if (prevTag == FT_CURVE_TAG_CONIC)
							{
								contour.push_back(FontGlyphVector(p, prev, FontGlyphVector::On));
								onCount++;
							}
						}
						contour.push_back(FontGlyphVector(p, FontGlyphVector::Conic));
						break;
					case FT_CURVE_TAG_CUBIC:
						contour.push_back(FontGlyphVector(p, FontGlyphVector::Cubic));
						break;
					default:
						contour.push_back(FontGlyphVector(p, FontGlyphVector::On));
						onCount++;
					}
					// todo: process outlines point
				}
				else
				{
					// error
				}
			}
			int contourStart = 0;
			if (onCount > 0)
			{
				while (contour[contourStart].tag != FontGlyphVector::On)
				{
					contour.push_back(contour[contourStart]);
					contourStart++;
				}
				glyph->addOutline(contourStart, contour);
			}
			start = end + 1;
		}
	}
}
