#include "FontsManager.h"

namespace fonts
{
	FontsManager FontsManager::Default;

	FontsManager::FontsManager(): _ftLibrary(nullptr)
	{
	}

	void FontsManager::init()
	{
		FT_Error error = FT_Init_FreeType(&_ftLibrary);
		if (error)
		{
			FontsManager::onFontError(error);
		}
	}

	void FontsManager::onFontError(int errorCode)
	{
		// todo: error handling
	}

	void* FontsManager::getFontLibraryInstance() const
	{
		return _ftLibrary;
	}

	OutlineVectorFont* FontsManager::createOutlineVectorFont(void* buffer, uint bytesCount, FontCharSet* charset) const
	{
		OutlineVectorFont* result = new OutlineVectorFont();
		result->load(buffer, bytesCount, charset);
		return result;
	}

	BitmapFont* FontsManager::createBitmapFont(void* buffer, uint bytesCount, float pixelSize, FontCharSet* charset) const
	{
		BitmapFont* result = new BitmapFont();
		result->load(buffer, bytesCount, pixelSize, charset);
		return result;
	}
}
