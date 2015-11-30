#ifndef ZOMBO_CONTENT_BLOCK_H
#define ZOMBO_CONTENT_BLOCK_H

#include "../zombo_classes.h"
#include "ZomboAtlas.h"

namespace zombo
{
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
		void update();
		void setRoot(const std::string &rootPath);
		typedef std::map<std::string, ZomboContentTexture*> TexturesMap;
		typedef std::map<std::string, ZomboContentAtlasPage*> AtlasPagesMap;
		typedef std::map<std::string, ZomboContentAtlas*> AtlasMap;
		typedef std::map<std::string, ZomboContentImage*> ImagesMap;
		typedef std::map<std::string, ZomboContentFrameAnimation*> FrameAnimationsMap;
	private:
		template <typename T> T* getMapValue(std::map<std::string, T*> map, const char * fileName) const;
		TexturesMap _textures;
		AtlasPagesMap _altasPages;
		AtlasMap _altases;
		ImagesMap _images;
		FrameAnimationsMap _frameAnimations;
		std::string _rootPath;
		std::queue<std::string> _loadingQueue;
		bool _loaded;
		
		static bool hasFormat(const std::string &fileName, const std::string& format);
		static std::string format(const std::string& fileName, int number, int leadingZeros);
		static std::string format(const std::string &fileName, int number);

		void loadResource(const std::string &fileName);
		void loadTexture(const std::string &fileName);
		ZomboContentAtlasPage* loadAtlasPage(const std::string &fileName);
		void loadAtlas(const std::string &fileName);
	};

	template <typename T>
	T* ZomboContentBlock::getMapValue(std::map<std::string, T*> map, const char* fileName) const
	{
		typename std::map<std::string, T*>::iterator it = map.find(fileName);
		if (it == map.end())
			return 0;
		return it->second;
	}
}

#endif /* ZOMBO_CONTENT_BLOCK_H */
