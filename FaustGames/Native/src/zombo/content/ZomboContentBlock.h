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
		ZomboContentTexture* getTexture(const char * fileName) const;
		ZomboContentAtlasPage* getAtlasPage(const char * fileName) const;
		ZomboContentAtlas* getAtlas(const char * fileName) const;
		void update();

		typedef std::map<std::string, ZomboContentTexture*> TexturesMap;
		typedef std::map<std::string, ZomboContentAtlasPage*> AtlasPagesMap;
		typedef std::map<std::string, ZomboContentAtlas*> AtlasMap;
	private:
		template <typename T> T* getMapValue(std::map<std::string, T*> map, const char * fileName) const;
		TexturesMap _textures;
		AtlasPagesMap _altasPages;
		AtlasMap _altases;

		std::queue<std::string> _loadingQueue;
		bool _loaded;
		
		static bool hasFormat(const std::string &fileName);
		static std::string format(const std::string &fileName, int number);

		void loadResource(const std::string &fileName);
		void loadTexture(const std::string &fileName);
		void loadAtlasPage(const std::string &fileName);
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
