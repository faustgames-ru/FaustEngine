#ifndef TEXTURES_MANAGER_H
#define TEXTURES_MANAGER_H

#include "resources_classes.h"
#include "ContentManager.h"
#include "TexturesLoader.h"
#include "AtlasPacker.h"

namespace resources
{
	class TextureEntry
	{
	public:
		std::string name;
		graphics::TextureImage2d* image;
		llge::TextureQueryFormat queryFormat;
		ImageInfo imageInfo;
		TextureEntry(const std::string &name, graphics::TextureImage2d* image, llge::TextureQueryFormat queryFormat);
		void loadImage(TexturesLoader* loader) const;
	};
	
	class TextureAtlasEntry
	{
	public:
		TextureEntry* entry;
		ImageInfo imageInfo;
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
		virtual void API_CALL fillStatistics(IntPtr statistics) override;
		virtual void API_CALL dispose() override;

		graphics::TextureImage2d* loadTextureImage(const char * name, llge::TextureQueryFormat queryFormat);
		void executeAtlasLoadingList();
		void executeLoadingList();
		void executeUnloadingList();
		void executeDisposeList();
		void clearAtlasPages();
		void loadEntry(TextureEntry* entry) const;
		static void unloadEntry(TextureEntry* entry);
		static void disposeEntry(TextureEntry* entry);
		static void unloadPage(graphics::TextureAtlasPage* page);
		static void disposePage(graphics::TextureAtlasPage* page);
	private:
		bool _atlasBuildStarted;
		TextureEntryMap _entries;
		std::vector<TextureEntry*> _loadingList;
		std::vector<TextureEntry*> _unloadingList;
		std::vector<TextureEntry*> _disposeList;		
		std::vector<TextureAtlasEntry> _atlasLoadingList;
		std::vector<graphics::TextureAtlasPage* > _atlasPages;
		TexturesLoader* _texturesLoader;
	};

	class TexturesAtlasPacker
	{
	public:
		static TexturesAtlasPacker* create(llge::TextureImage2dFormat format);
		explicit TexturesAtlasPacker(llge::TextureImage2dFormat format, IAtlasPlacer* placer);
		void pack(TexturesLoader* texturesLoader, const std::vector<TextureAtlasEntry> &entries, std::vector<graphics::TextureAtlasPage*> &pages);
	protected:
		AlignInfo alignInfo;
		llge::TextureImage2dFormat _internalFormat;
		IAtlasPlacer* placer;
	};
}

#endif /*TEXTURES_MANAGER_H*/
