#ifndef ZOMBO_CONTENT_BLOCK_H
#define ZOMBO_CONTENT_BLOCK_H

#include "../zombo_classes.h"
#include "ZomboAtlas.h"
#include "ZomboBitmapFont.h"
#include "ZomboContentScene.h"

namespace zombo
{
	class ZomboContentGame;

	// todo: unloading
	class ZomboContentBlock
	{
	public:
		ZomboContentBlock();
		~ZomboContentBlock();
		void enqueueResource(const char * fileName);
		bool isLoaded() const;
		ZomboContentFrameAnimation* createAnimation(const char * fileName) const;

		ZomboContentImage* getImage(const char * fileName) const;
		ZomboContentTexture* getTexture(const char * fileName) const;
		ZomboContentAtlasPage* getAtlasPage(const char * fileName) const;
		ZomboContentAtlas* getAtlas(const char * fileName) const;
		ZomboBitmapFont* getBitmapFont(const char* fileName) const;
		ZomboContentGame* getGame(const char* fileName) const;
		ZomboContentScene* getScene(const char* fileName) const;
		ZomboContentPlatform* getPlatform(const char* fileName) const;
		void update();
		void setRoot(const std::string &rootPath);
		typedef std::map<std::string, ZomboContentTexture*> TexturesMap;
		typedef std::map<std::string, ZomboContentAtlasPage*> AtlasPagesMap;
		typedef std::map<std::string, ZomboContentAtlas*> AtlasMap;
		typedef std::map<std::string, ZomboContentImage*> ImagesMap;
		typedef std::map<std::string, ZomboContentFrameAnimation*> FrameAnimationsMap;
		typedef std::map<std::string, ZomboBitmapFont*> BitmapFontsMap;
		typedef std::map<std::string, ZomboContentGame*> GamesMap;
		typedef std::map<std::string, ZomboContentScene*> ScenesMap;
		typedef std::map<std::string, ZomboContentPlatform*> PlatformsMap;
	private:
		template <typename T> T* getMapValue(std::map<std::string, T*> map, const char * fileName) const;
		TexturesMap _textures;
		AtlasPagesMap _altasPages;
		AtlasMap _altases;
		ImagesMap _images;
		FrameAnimationsMap _frameAnimations;
		BitmapFontsMap _bitmapFonts;
		GamesMap _games;
		ScenesMap _scenes;
		PlatformsMap _platforms;
		std::string _rootPath;
		std::queue<std::string> _loadingQueue;
		bool _loaded;

		std::string getBitmapFontFileName(const std::string& fileName);
		void getBitmapFontConfig(const std::string& fileName, int &size, fonts::FontCharSet* &charset);
		int getBitmapFontSize(const std::string& fileName);
		static bool hasFormat(const std::string &fileName, const std::string& format);
		static std::string format(const std::string& fileName, int number, int leadingZeros);
		static std::string format(const std::string &fileName, int number);

		void loadResource(const std::string &fileName);
		ZomboContentPlatform* loadPlatform(const std::string &fileName);
		ZomboContentScene* loadScene(const std::string &fileName);
		ZomboContentGame* loadGame(const std::string &fileName);
		ZomboContentImage* loadTexture(const std::string &fileName);
		ZomboContentAtlasPage* loadAtlasPage(const std::string &fileName);
		void loadAtlas(const std::string &fileName);
		void loadBitmapFont(const std::string &fileName, int size, fonts::FontCharSet* charset);
	};

	template <typename T>
	T* ZomboContentBlock::getMapValue(std::map<std::string, T*> map, const char* fileName) const
	{
		typename std::map<std::string, T*>::iterator it = map.find(fileName);
		if (it == map.end())
			return nullptr;
		return it->second;
	}
}

#endif /* ZOMBO_CONTENT_BLOCK_H */
