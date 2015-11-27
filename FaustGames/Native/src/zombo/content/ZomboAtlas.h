#ifndef ZOMBO_ATLAS_H
#define ZOMBO_ATLAS_H

#include "../zombo_classes.h"

namespace zombo
{
	struct ZomboImageVertex
	{
		core::Vector2 xy;
		core::Vector2 uv;
	};
	class ZomboContentImage
	{
	public:
		uint textureId;
		std::string name;
		ZomboRect origins;
		ZomboRect region;
		core::Vector2 pivot;
		std::vector<ZomboImageVertex> vertices;
		std::vector<short> indices;		
	};

	class ZomboContentTexture
	{
	public:
		uint textureId;
		std::string fileName;
	};

	class ZomboContentAtlasPage
	{
	public:
		ZomboContentTexture texture;
		std::vector<ZomboContentImage *> images;
	};

	class ZomboContentAtlas
	{
	public:
		std::string name;
		std::vector<ZomboContentAtlasPage *> pages;
	};

	class ZomboContentFrameAnimation
	{
	public:
		std::vector<ZomboContentImage *> frames;
	};
}

#endif /* ZOMBO_ATLAS_H */