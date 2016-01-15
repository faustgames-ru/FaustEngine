#include "BitmapFont.h"
#include "FontsManager.h"
#include "FontRects.h"
#include "../resources/ContentManager.h"

namespace fonts
{
	BitmapFont::BitmapFont()
	{
		for (uint i = 0; i < GlyphsCount; i++)
		{
			_glyphs[i] = nullptr;
		}
	}

	BitmapFont::~BitmapFont()
	{
		for (uint i = 0; i < GlyphsCount; i++)
		{
			if (_glyphs[i] != nullptr)
			{
				delete _glyphs[i];
			}
		}
	}

	void BitmapFont::render(const core::Vector3& position, float scale, const char* text, IBitmapFontRenderer* renderer)
	{
		if (text == nullptr) return;
		core::Vector3 pen = position;
		FontGlyphSegment seg;
		for (uint i = 0; text[i] != 0; i++)
		{
			BitmapFontGlyph* glyph = _glyphs[text[i]];
			if (glyph != nullptr)
			{
				if (glyph->pageIndex >= 0)
				{
					BitmapFontVertex min = glyph->min;
					min.xy *= scale;
					min.xy += pen.toVector2();
					BitmapFontVertex max = glyph->max;
					max.xy *= scale;
					max.xy += pen.toVector2();
					renderer->drawQuad(0xffffffff, _textures[glyph->pageIndex], min, max, pen.getZ(), glyph->rotation);
				}
				pen.setX(pen.getX() + glyph->advance.getX() * scale);
			}
		}
	}

	void BitmapFont::load(void* inBuffer, int bytesCount, float pixelSize, FontCharSet* charset)
	{
		// allow use resources buffer for image allocations
		void* buffer = core::Mem::allocate(bytesCount); 
		memcpy(buffer, inBuffer, bytesCount);
		std::vector<char> _empties;
		FT_Library library = static_cast<FT_Library>(FontsManager::Default.getFontLibraryInstance());
		FT_Byte* byteBuffer = static_cast<FT_Byte*>(buffer);
		FT_Face face;
		FT_Error error = FT_New_Memory_Face(library, byteBuffer, bytesCount, 0, &face);
		if (error)
		{
			FontsManager::Default.onFontError(error);
			return;
		}

		int size = static_cast<int>(core::Math::round(pixelSize * 64.0f));

		FT_GlyphSlot slot = face->glyph;
		error = FT_Set_Char_Size(face, 0, size, 72, 72);
		std::vector<FontRectSize> sizes;
		char symbol;
		for (uint n = 0; n < charset->charset.size(); n++)
		{
			symbol = charset->charset[n];
			FT_UInt glyph_index = FT_Get_Char_Index(face, symbol);
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error)
			{
				continue;
			}

			FontRectSize rect_size = FontRectSize(symbol, slot->metrics.width / 64, slot->metrics.height / 64);
			if (!rect_size.empty())
			{
				sizes.push_back(rect_size);
			}		
			else
			{
				_empties.push_back(symbol);
			}
			// todo: place rect in bin pack
		}

		std::vector<FontRectsPage> pages;
		FontRects::build(128, 128, sizes, pages);
		ushort maxT = 65535;
		for (uint i = 0; i < pages.size(); i++)
		{
			FontRectsPage& p = pages[i];
			uint w = p.w;
			uint h = p.h;
			uint *imageBuffer = static_cast<uint *>(resources::ContentManager::Default.getBuffer());
			graphics::TextureImage2d *texture = new graphics::TextureImage2d(false, false);
			_textures.push_back(texture);
			memset(imageBuffer, 0, w*h * 4);
			BitmapFontGlyph *glyph;

			for (uint j = 0; j < p.rects.size(); j++)
			{
				FontRect &r = p.rects[j];
				symbol = r.symbol;
				FT_UInt glyph_index = FT_Get_Char_Index(face, symbol);
				error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
				if (error)
				{
					continue;
				}
				error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
				if (error)
					continue;

				switch (slot->format)
				{
				case FT_GLYPH_FORMAT_NONE:
					break;
				case FT_GLYPH_FORMAT_BITMAP:
					glyph = new BitmapFontGlyph();
					glyph->advance = core::Vector2(static_cast<float>(slot->advance.x) / 64.0f, static_cast<float>(slot->advance.y) / 64.0f);
					glyph->pageIndex = -1;
					glyph->rotation = false;
					if (slot->bitmap.width == r.w && slot->bitmap.rows == r.h)
					{
						glyph->pageIndex = _textures.size() - 1;						
						unsigned char *srcRow = static_cast<unsigned char *>(slot->bitmap.buffer);
						uint *dstRow = imageBuffer + r.y * w + r.x;
						for (uint y = 0; y < r.h; y++)
						{
							for (uint x = 0; x < r.w; x++)
							{
								*(dstRow + x) = 0x00ffffff | *(srcRow+x) << 24;
							}
							srcRow += slot->bitmap.pitch;
							dstRow += w;
						}

						glyph->min.xy = core::Vector2(static_cast<float>(slot->metrics.horiBearingX) / 64.0f, static_cast<float>(slot->metrics.horiBearingY - slot->metrics.height) / 64.0f);
						glyph->min.u = r.x * maxT / w;
						glyph->min.v = (r.y + r.h) * maxT / h;
						glyph->max.xy = core::Vector2(static_cast<float>(slot->metrics.horiBearingX + slot->metrics.width) / 64.0f, static_cast<float>(slot->metrics.horiBearingY) / 64.0f);
						glyph->max.u = (r.x + r.w) * maxT / w;
						glyph->max.v = r.y * maxT / h;
					}
					else if (slot->bitmap.width == r.h && slot->bitmap.rows == r.w)
					{
						glyph->pageIndex = _textures.size() - 1;

						// rotate
						unsigned char *srcRow = static_cast<unsigned char *>(slot->bitmap.buffer);
						unsigned char *src;
						uint *dstRow = imageBuffer + r.y * w + r.x;
						for (uint y = 0; y < r.h; y++)
						{
							for (uint x = 0; x < r.w; x++)
							{
								src = (srcRow + x*slot->bitmap.pitch) + y;
								*(dstRow + x) = 0x00ffffff | *src << 24;
							}
							dstRow += w;
						}

						glyph->min.xy = core::Vector2(static_cast<float>(slot->metrics.horiBearingX) / 64.0f, static_cast<float>(slot->metrics.horiBearingY - slot->metrics.height) / 64.0f);
						glyph->min.u = r.x * maxT / w ;
						glyph->min.v = r.y* maxT / h;
						glyph->max.xy = core::Vector2(static_cast<float>(slot->metrics.horiBearingX + slot->metrics.width) / 64.0f, static_cast<float>(slot->metrics.horiBearingY) / 64.0f);
						glyph->max.u = (r.x + r.w) * maxT / w;
						glyph->max.v = (r.y + r.h) * maxT / h;
						glyph->rotation = true;
					}
					else
					{
						//  error;
						slot = slot;
					}
					_glyphs[r.symbol] = glyph;

					// todo: map symbols
					break;
				case FT_GLYPH_FORMAT_PLOTTER:
					break;
				case FT_GLYPH_FORMAT_OUTLINE:
					break;
				case FT_GLYPH_FORMAT_COMPOSITE:
					break;
				}
			}
			texture->create();
			texture->setData(w, h, graphics::Image2dFormat::Rgba, imageBuffer);
		}
		for (uint i = 0; i < _empties.size(); i++)
		{
			symbol = _empties[i];
			FT_UInt glyph_index = FT_Get_Char_Index(face, symbol);
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error)
			{
				continue;
			}
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
				continue;
			BitmapFontGlyph *glyph = new BitmapFontGlyph();
			glyph->advance = core::Vector2(static_cast<float>(slot->advance.x) / 64.0f, static_cast<float>(slot->advance.y) / 64.0f);
			glyph->pageIndex = -1;
			_glyphs[symbol] = glyph;
		}
		FT_Done_Face(face);
		core::Mem::deallocate(buffer);
	}

	void BitmapFont::unload()
	{
		for (uint i = 0; i < _textures.size(); i++)
		{
			_textures[i]->cleanup();
			delete _textures[i];
		}
	}
}

