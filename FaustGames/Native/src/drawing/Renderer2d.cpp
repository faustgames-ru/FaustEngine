#include "Renderer2d.h"

namespace drawing
{
	Renderer2d::Renderer2d() : _format(graphics::VertexFormats::positionTextureColor())
	{
		_vertices = (Mesh2dVertex *)malloc(VerticesPerDraw * sizeof(Mesh2dVertex));
		_indices = (unsigned short *)malloc(IndicesPerDraw * sizeof(unsigned short));
		_graphicsDevice = &graphics::GraphicsDevice::Default;
	}

	void Renderer2d::start()
	{
		_drawCalls = 0;
		_graphicsDevice->clear();
		_graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
		_graphicsDevice->renderState.setDepth(graphics::DepthState::None);
		_graphicsDevice->renderState.setEffect(graphics::Effects::textureColor());
		_projectionContainer.setValue(Camera.getMatrix().getData());
		graphics::UniformValues::projection()->setValue(_projectionContainer);

		_verticesCount = 0;
		_indicesCount = 0;
		_texture = 0;
	}

	int Renderer2d::finish()
	{
		draw();
		return _drawCalls;
	}

	void Renderer2d::drawMesh(unsigned int texture, Mesh2dVertex *vertices, int verticesCount, unsigned short *indices, int indicesCount)
	{
		//graphics::UniformValues::texture()->setValue(texture);
		//_graphicsDevice->drawPrimitives(_format, vertices, indices, indicesCount / 3);
		//return;

		if (verticesCount == 0) return;
		if (indicesCount == 0) return;

		if ((_texture != texture) ||
			((_verticesCount + verticesCount) > VerticesPerDraw) ||
			((_indicesCount + indicesCount) > IndicesPerDraw))
		{
			draw();
			_texture = texture;
		}
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = (unsigned short)(_verticesCount + indices[i]);
		}
		Mesh2dVertex* target = _vertices +  _verticesCount;
		Mesh2dVertex* source = vertices;
		for (int i = 0; i < verticesCount; i++, target++, source++)
		{
			Transform.transform<Mesh2dVertex>(target, source);
			target->u = source->u;
			target->v = source->v;
			target->color = source->color;
			
		}
		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}

	void Renderer2d::draw()
	{
		if (_verticesCount == 0) return;
		if (_indicesCount == 0) return;

		graphics::UniformValues::texture()->setValue(_texture);
		_graphicsDevice->drawPrimitives(_format, _vertices, _indices, _indicesCount / 3);
		_drawCalls++;

		_verticesCount = 0;
		_indicesCount = 0;
		_texture = 0;

	}

}