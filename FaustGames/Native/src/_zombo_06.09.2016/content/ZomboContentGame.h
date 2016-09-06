#ifndef ZOMBO_CONTENT_GAME_H
#define ZOMBO_CONTENT_GAME_H

#include "../zombo_classes.h"
#include "ZomboContentBlock.h"
#include "../common/ZomboCamera.h"

namespace zombo
{

	class ZomboContentGame
	{
	public:
		std::string thisDir;
		std::string startup;

		static ZomboContentGame* createFromJson(const char *jsonString);
		void dispose() const;
	};
}

#endif /* ZOMBO_CONTENT_GAME_H */