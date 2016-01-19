#ifndef ZOMBO_ATLAS_H
#define ZOMBO_ATLAS_H

#include "../zombo_classes.h"

namespace zombo
{
	struct ZomboImageVertex
	{
		core::Vector2 xy;
		ushort u;
		ushort v;
	};
	class ZomboContentImage
	{
	public:
		graphics::TextureImage2d *texture;
		std::string name;
		std::vector<ZomboImageVertex> vertices;
		std::vector<short> indices;		
		geometry::Aabb2d bounds;
		void initWithQuad(float w, float h);
		static ZomboContentImage * create();
		void dispose() const;
	};

	class ZomboContentTexture
	{
	public:
		graphics::TextureImage2d *texture;
		std::string fileName;
	};

	class ZomboContentAtlasPage
	{
	public:
		ZomboContentTexture texture;
		std::vector<ZomboContentImage *> images;
		
		static ZomboContentAtlasPage * create();
		void dispose() const;
		
		ZomboContentAtlasPage();
		~ZomboContentAtlasPage();
	};

	class ZomboContentAtlas
	{
	public:
		std::string name;
		std::vector<ZomboContentAtlasPage *> pages;
		static ZomboContentAtlas * create();
		void dispose() const;
	};

	class ZomboContentFrameAnimation
	{
	public:
		std::vector<ZomboContentImage *> frames;
		static ZomboContentFrameAnimation* create();
		void dispose() const;
	};
}

#endif /* ZOMBO_ATLAS_H */