#include "RenderMesh2d.h"

namespace drawing
{
	RenderMesh2d::RenderMesh2d() : _vertices(0), _indices(0), _verticesCount(0), _indicesCount(0), _texture(0)
	{
	}

	RenderMesh2d::~RenderMesh2d()
	{
		if (_vertices)
			core::Mem::deallocate(_vertices);
		if (_indices)
			core::Mem::deallocate(_indices);
	}


	void RenderMesh2d::setData(llge::ITexture * texture, Mesh2dVertex* vertices, int verticesCount, unsigned short* indices, int indicesCount)
	{
		_texture = texture;
		reallocVertices(verticesCount);
		reallocIndices(indicesCount);
		memcpy(_vertices, vertices, verticesCount * sizeof(Mesh2dVertex));
		memcpy(_indices, indices, indicesCount * sizeof(unsigned short));
	}
	
	void RenderMesh2d::render(Renderer2d *renderer)
	{
		renderer->drawMesh(_texture->getId(), _vertices, _verticesCount, _indices, _indicesCount);
	}

	void RenderMesh2d::reallocVertices(int verticesCount)
	{
		if (_vertices)
			core::Mem::deallocate(_vertices);
		_vertices = (Mesh2dVertex *)core::Mem::allocate(verticesCount * sizeof(Mesh2dVertex));
		_verticesCount = verticesCount;
	}
	void RenderMesh2d::reallocIndices(int indicesCount)
	{
		if (_indices)
			core::Mem::deallocate(_indices);
		_indices = (unsigned short *)core::Mem::allocate(indicesCount * sizeof(unsigned short));
		_indicesCount = indicesCount;
	}

}