#ifndef CONTENT_IMAGE_H
#define CONTENT_IMAGE_H

#include "content_classes.h"

namespace content
{
	class ContentImage : public IContent
	{
	public:
		graphics::TextureImage2d *texture;
		std::string name;
		std::vector<ImageVertex> vertices;
		std::vector<ushort> indices;
		geometry::Aabb2d bounds;
		void initWithQuad(float w, float h, uint detail);		
		static ContentImage * create();
		void internalLoad(const LoadArgs& e);
		virtual void dispose() OVERRIDE;
		virtual void load(const LoadArgs& e) OVERRIDE;
	private:
		ContentImage();
		~ContentImage();
	};
}

#endif /*CONTENT_IMAGE_H*/
