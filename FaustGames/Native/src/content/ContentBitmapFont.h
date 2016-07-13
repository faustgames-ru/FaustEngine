#ifndef CONTENT_BITMAP_FONT_H
#define CONTENT_BITMAP_FONT_H

#include "content_classes.h"

namespace content
{
	class ContentBitmapFont : public IContent
	{
	public:
		fonts::BitmapFont *font;
		static ContentBitmapFont * create();
		virtual void dispose() OVERRIDE;
		virtual void load(const LoadArgs& e) OVERRIDE;
	private:
		ContentBitmapFont();
		~ContentBitmapFont();
	};
}

#endif /*CONTENT_BITMAP_FONT_H*/

