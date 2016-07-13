#ifndef CONTENT_BLOCK_H
#define CONTENT_BLOCK_H

#include "content_classes.h"
#include "ContentBitmapFont.h"
#include "ContentStatus.h"

namespace content
{
	// todo: unloading
	class ContentBlock
	{
	public:
		ContentBlock();
		float ContentScale;
		void setRoot(const std::string &rootPath);
		ContentStatus* enqueueResource(const char * fileName);
		void update();
		typedef std::map<std::string, ContentStatus *> ResourcesMap;
		typedef std::queue<ContentStatus *> LoadingQueue;
	private:
		template <typename T> T* getMapValue(std::map<std::string, T*> map, const char * fileName) const;
		ResourcesMap _resources;
		
		std::string _rootPath;
		std::queue<ContentStatus*> _loadingQueue;
		LoadArgs _loadArgs;
	};

	template <typename T>
	T* ContentBlock::getMapValue(std::map<std::string, T*> map, const char* fileName) const
	{
		typename std::map<std::string, T*>::iterator it = map.find(fileName);
		if (it == map.end())
			return nullptr;
		return it->second;
	}
}

#endif /* CONTENT_BLOCK_H */
