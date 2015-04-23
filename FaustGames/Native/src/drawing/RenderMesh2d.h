#ifndef RENDER_MESH_2D_H
#define RENDER_MESH_2D_H

#include "drawing_classes.h"
#include "RenderItem2d.h"

namespace drawing
{
	class RenderMesh2d : public RenderItem2d
	{
	public:
		RenderMesh2d();
		~RenderMesh2d();
		void setData(llge::ITexture * texture, Mesh2dVertex* vertices, int verticesCount, unsigned short* indices, int indicesCount);
		virtual void render(Renderer2d *renderer);
	private:
		void reallocVertices(int verticesCount);
		void reallocIndices(int indicesCount);

		Mesh2dVertex* _vertices;
		unsigned short* _indices;
		int _verticesCount;
		int _indicesCount;
		llge::ITexture *_texture;
	};
}

#endif /*RENDER_MESH_2D_H*/