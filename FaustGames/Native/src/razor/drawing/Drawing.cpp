#include "Drawing.h"
#include "../../graphics/pipelines/RenderService.h"

namespace razor
{
	SpriteTransform::SpriteTransform(): s(0), c(1)
	{
	}

	SpriteTransform::SpriteTransform(core::Vector3 p) : s(0), c(1)
	{
		position = p;
	}

	SpriteTransform::SpriteTransform(core::Vector3 p, float scale) : s(0), c(scale)
	{
		position = p;
	}

	SpriteTransform::SpriteTransform(core::Vector3 p, float scale, float rotation) : 
		s(core::Math::sin(rotation)*scale), 
		c(core::Math::cos(rotation)*scale)
	{
		position = p;
	}

	core::Vector3 SpriteTransform::tansform(core::Vector3 p) const
	{
		return core::Vector3(
			position.getX() + p.getX()*c - p.getY()*s,
			position.getY() + p.getX()*s + p.getY()*c,
			position.getZ() + p.getZ());
	}

	core::Vector3 SpriteTransform::tansform(core::Vector2 p) const
	{
		return core::Vector3(
			position.getX() + p.getX()*c - p.getY()*s,
			position.getY() + p.getX()*s + p.getY()*c,
			position.getZ());
	}

	graphics::SpriteVertex _vertices[64];

	Drawing Drawing::Default;

	void Drawing::drawSprite(const SpriteTransform& transform, content::ContentImage* sprite)
	{
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;

		for (uint i = 0; i < sprite->vertices.size(); i++)
		{
			_vertices[i].xyz = transform.tansform(sprite->vertices[i].xy);
			_vertices[i].color = 0xff808080;
			_vertices[i].u = sprite->vertices[i].u;
			_vertices[i].v = sprite->vertices[i].v;
		}

		graphics::RenderPipelineInput<graphics::SpriteVertex, graphics::UniformConfigTexture> input;
		input.verticesData = _vertices;
		input.verticesCount = sprite->vertices.size();
		input.effect = graphics::EffectsBasic::textureColor();
		input.indicesData = sprite->indices.data();
		input.indicesCount = sprite->indices.size();
		input.uniforms.testureId = sprite->texture->getHandle();
		pipeline->draw(input);
	}
}