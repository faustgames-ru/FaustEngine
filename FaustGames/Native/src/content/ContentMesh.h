#ifndef CONTENT_MESH_H
#define CONTENT_MESH_H

#include "content_classes.h"
#include "../geometry/Aabb.h"

namespace content
{

	class ContentMesh : public IContent
	{
	public:
		graphics::TextureImage2d *diffuse;
		std::vector<MeshVertex> vertices;
		std::vector<ushort> indices;
		geometry::Aabb bounds;
		static ContentMesh * create();
		void internalLoad(const LoadArgs& e);
		virtual void dispose() OVERRIDE;
		virtual void load(const LoadArgs& e) OVERRIDE;
	private:
		ContentMesh();
		~ContentMesh();
	};
}

#endif /*CONTENT_MESH_H*/
