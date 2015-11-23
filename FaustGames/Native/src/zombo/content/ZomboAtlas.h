#ifndef ZOMBO_ATLAS_H
#define ZOMBO_ATLAS_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboAtlas
	{
	public:
		std::vector<uint> pagesIds;
		std::vector<ZomboSpriteImage> regions;
	};
}

#endif /* ZOMBO_ATLAS_H */