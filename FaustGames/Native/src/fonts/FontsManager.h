#ifndef FONTS_FONTS_MANAGER_H
#define FONTS_FONTS_MANAGER_H

#include "fonts_classes.h"
#include "OutlineVectorFont.h"
#include "BitmapFont.h"

namespace fonts
{
	class FontsManager
	{
	public:
		static FontsManager Default;
		FontsManager();
		void init();
		static void onFontError(int errorCode);
		void *getFontLibraryInstance() const;

		OutlineVectorFont *createOutlineVectorFont(const void *buffer, uint bytesCount, FontCharSet *charset) const;
		BitmapFont *createBitmapFont(const void *buffer, uint bytesCount, float pixelSize, FontCharSet *charset) const;
	private:
		FT_Library _ftLibrary;
	};
}

#endif /*FONTS_FONTS_MANAGER_H*/