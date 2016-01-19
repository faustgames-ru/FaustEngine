#ifndef ZOMBO_CONTENT_SCENE_H
#define ZOMBO_CONTENT_SCENE_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboContentPlatform
	{
	public:
		std::string thisDir;
		std::vector<std::string> left;
		std::vector<std::string> middle;
		std::vector<std::string> right;

		static ZomboContentPlatform* createFromJson(const char *jsonString);
		void dispose() const;
	};

	class ZomboContentScene
	{
	public:
		std::string thisDir;
		std::vector<std::string> resources;
		std::vector<std::string> platforms;
		static ZomboContentScene* createFromJson(const char* jsonString);
		void dispose() const;
	};
}

#endif /* ZOMBO_CONTENT_SCENE_H */