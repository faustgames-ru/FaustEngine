#ifndef TEXTURES_MANAGER_H
#define TEXTURES_MANAGER_H

#include "resources_classes.h"
#include "ContentManager.h"
#include "TexturesLoader.h"

namespace resources
{
	class TextureEntry
	{
	public:
		std::string name;
		graphics::TextureImage2d* image;
		llge::TextureQueryFormat queryFormat;
		ImageInfo* imageInfo;
		TextureEntry(const std::string &name, graphics::TextureImage2d* image, llge::TextureQueryFormat queryFormat);
		void loadImage(TexturesLoader* loader) const;
	};

	typedef std::map<std::string, TextureEntry*> TextureEntryMap;

	class TexturesManager : public llge::ITexturesManager
	{
	public:
		TexturesManager(TexturesLoader* texturesLoader);
		~TexturesManager();		
		virtual void API_CALL startAtlasBuild() override;
		virtual void API_CALL finishAtlasBuild() override;
		virtual llge::ITextureImage2d* API_CALL loadImage(const char * name, llge::TextureQueryFormat queryFormat) override;
		virtual void API_CALL update() override;
		// 
		virtual void API_CALL clear() override;
		virtual void API_CALL dispose() override;

		graphics::TextureImage2d* loadTextureImage(const char * name, llge::TextureQueryFormat queryFormat);
		void executeLoadingList();
		void executeUnloadingList();
		void executeDisposeList();
		void loadEntry(TextureEntry* entry);
		void unloadEntry(TextureEntry* entry);
		void disposeEntry(TextureEntry* entry);
	private:
		bool _atlasBuildStarted;
		TextureEntryMap _entries;
		std::vector<TextureEntry*> _loadingList;
		std::vector<TextureEntry*> _unloadingList;
		std::vector<TextureEntry*> _disposeList;		
		TexturesLoader* _texturesLoader;
	};
}

#endif /*TEXTURES_MANAGER_H*/
