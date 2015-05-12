#ifndef SPINE_ATLAS_H
#define SPINE_ATLAS_H

#include "SkeletonResource.h"

namespace spine
{
	class Atlas
	{
	public:
		Atlas();
		~Atlas();
		void create();
		void cleanup();
	protected:
	private:
		void releaseAtlas();
		spAtlas * _spAtlas;
	};
}

#endif /*SPINE_ATLAS_H*/