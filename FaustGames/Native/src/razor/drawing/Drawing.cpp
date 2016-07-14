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

	Pen::Pen(): color(0xffffffff), width(0.02f)
	{
	}

	Drawing Drawing::Default;

	Drawing::Drawing()
	{
		_quadIndices[0] = 0;
		_quadIndices[1] = 1;
		_quadIndices[2] = 2;
		_quadIndices[3] = 0;
		_quadIndices[4] = 2;
		_quadIndices[5] = 3;
	}

	void Drawing::drawSprite(const SpriteTransform& transform, content::ContentImage* sprite)
	{
		drawSprite(transform, 0xff808080, sprite);
	}

	void Drawing::drawSprite(const SpriteTransform& transform, uint color, content::ContentImage* sprite)
	{
		_spriteVertices.resize(sprite->vertices.size());
		for (uint i = 0; i < sprite->vertices.size(); i++)
		{
			_spriteVertices[i].xyz = transform.tansform(sprite->vertices[i].xy);
			_spriteVertices[i].color = color;
			_spriteVertices[i].u = sprite->vertices[i].u;
			_spriteVertices[i].v = sprite->vertices[i].v;
		}

		graphics::RenderPipelineInput<graphics::SpriteVertex, graphics::UniformConfigTexture> input;
		input.verticesData = _spriteVertices.data();
		input.verticesCount = sprite->vertices.size();
		input.effect = graphics::EffectsBasic::textureColor();
		input.indicesData = sprite->indices.data();
		input.indicesCount = sprite->indices.size();
		input.uniforms.testureId = sprite->texture->getHandle();
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;
		pipeline->draw(input);
	}

	void Drawing::drawAabb(const geometry::Aabb& aabb)
	{
		drawLine(aabb.Min.getX(), aabb.Min.getY(), aabb.Min.getZ(), aabb.Max.getX(), aabb.Min.getY(), aabb.Min.getZ());
		drawLine(aabb.Max.getX(), aabb.Min.getY(), aabb.Min.getZ(), aabb.Max.getX(), aabb.Max.getY(), aabb.Min.getZ());
		drawLine(aabb.Max.getX(), aabb.Max.getY(), aabb.Min.getZ(), aabb.Min.getX(), aabb.Max.getY(), aabb.Min.getZ());
		drawLine(aabb.Min.getX(), aabb.Max.getY(), aabb.Min.getZ(), aabb.Min.getX(), aabb.Min.getY(), aabb.Min.getZ());

		drawLine(aabb.Min.getX(), aabb.Min.getY(), aabb.Max.getZ(), aabb.Max.getX(), aabb.Min.getY(), aabb.Max.getZ());
		drawLine(aabb.Max.getX(), aabb.Min.getY(), aabb.Max.getZ(), aabb.Max.getX(), aabb.Max.getY(), aabb.Max.getZ());
		drawLine(aabb.Max.getX(), aabb.Max.getY(), aabb.Max.getZ(), aabb.Min.getX(), aabb.Max.getY(), aabb.Max.getZ());
		drawLine(aabb.Min.getX(), aabb.Max.getY(), aabb.Max.getZ(), aabb.Min.getX(), aabb.Min.getY(), aabb.Max.getZ());

		drawLine(aabb.Min.getX(), aabb.Min.getY(), aabb.Min.getZ(), aabb.Min.getX(), aabb.Min.getY(), aabb.Max.getZ());
		drawLine(aabb.Min.getX(), aabb.Max.getY(), aabb.Min.getZ(), aabb.Min.getX(), aabb.Max.getY(), aabb.Max.getZ());
		drawLine(aabb.Max.getX(), aabb.Max.getY(), aabb.Min.getZ(), aabb.Max.getX(), aabb.Max.getY(), aabb.Max.getZ());
		drawLine(aabb.Max.getX(), aabb.Min.getY(), aabb.Min.getZ(), aabb.Max.getX(), aabb.Min.getY(), aabb.Max.getZ());
	}

	void Drawing::drawLine(const core::Vector3& a, const core::Vector3& b)
	{
		core::Vector2 a2 = a.toVector2();
		core::Vector2 b2 = b.toVector2();
		float hw = pen.width*0.5f;
		core::Vector3 n;
		if (core::Vector2::equals(a2, b2))
		{
			n = core::Vector3(1, 0, 0)*hw;
		}
		else
		{
			n = (b.toVector2() - a.toVector2()).normalize().rotate90cw().toVector3()*hw;
		}
		_solidVertices.resize(4);
		_solidVertices[0].xyz = a + n;
		_solidVertices[1].xyz = b + n;
		_solidVertices[2].xyz = b - n;
		_solidVertices[3].xyz = a - n;
		for (int i = 0; i < 4; i++)
		{
			_solidVertices[i].color = pen.color;
		}

		graphics::RenderPipelineInput<graphics::ColorVertex, graphics::UniformConfigNone> input;
		input.verticesData = _solidVertices.data();
		input.verticesCount = 4;
		input.effect = graphics::EffectsBasic::solidColor();
		input.indicesData = _quadIndices;
		input.indicesCount = 6;
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;
		pipeline->draw(input);
	}

	void Drawing::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		drawLine(core::Vector3(x1, y1, z1), core::Vector3(x2, y2, z2));
	}
}