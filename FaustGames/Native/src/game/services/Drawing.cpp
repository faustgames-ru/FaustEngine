#include "Drawing.h"
#include "../../graphics/pipelines/RenderService.h"
#include "../../content/ContentImage.h"
#include "../../content/ContentMesh.h"

namespace game
{
	Rotation::Rotation() : s(0), c(1)
	{
	}

	Rotation::Rotation(float scale) : s(0), c(scale)
	{
	}

	Rotation::Rotation(float scale, float rotation): s(core::Math::sin(rotation)*scale), c(core::Math::cos(rotation)*scale)
	{
	}

	SpriteTransform::SpriteTransform()
	{
	}

	SpriteTransform::SpriteTransform(core::Vector3 p): position(p)
	{
	}

	SpriteTransform::SpriteTransform(core::Vector3 p, float scale) : position(p), rotation(scale)
	{
	}

	SpriteTransform::SpriteTransform(core::Vector3 p, float scale, float rotation) : position(p), rotation(scale, rotation)
	{
	}

	core::Vector3 SpriteTransform::tansform(core::Vector3 p) const
	{
		return core::Vector3(
			position.getX() + p.getX()*rotation.c - p.getY()*rotation.s,
			position.getY() + p.getX()*rotation.s + p.getY()*rotation.c,
			position.getZ() + p.getZ());
	}

	core::Vector3 SpriteTransform::tansform(core::Vector2 p) const
	{
		return core::Vector3(
			position.getX() + p.getX()*rotation.c - p.getY()*rotation.s,
			position.getY() + p.getX()*rotation.s + p.getY()*rotation.c,
			position.getZ());
	}

	MeshTransform::MeshTransform(): position(core::Vector3::empty), rotation(core::Matrix3::identity)
	{
	}

	Pen::Pen() : color(0xffffffff), width(0.02f)
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

	void Drawing::drawMesh(const MeshTransform& transform, content::ContentMesh* mesh)
	{
		_spriteVertices.resize(mesh->vertices.size());
		for (uint i = 0; i < mesh->vertices.size(); i++)
		{
			_spriteVertices[i].xyz = core::Matrix3::transform(transform.rotation, mesh->vertices[i].xyz) + transform.position;
			_spriteVertices[i].color = 0xff808080;
			_spriteVertices[i].u = mesh->vertices[i].u;
			_spriteVertices[i].v = mesh->vertices[i].v;
		}

		graphics::RenderPipelineInput<graphics::SpriteVertex, graphics::UniformConfigTexture> input;
		input.verticesData = _spriteVertices.data();
		input.verticesCount = mesh->vertices.size();
		input.effect = graphics::EffectsBasic::textureColor();
		input.indicesData = mesh->indices.data();
		input.indicesCount = mesh->indices.size();
		input.uniforms.testureId = mesh->diffuse->getHandle();
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;
		pipeline->draw(input);
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

	void Drawing::fillRect(const core::Vector3& c, const core::Vector2& s)
	{
		_solidVertices.resize(4);
		_solidVertices[0].xyz = core::Vector3(c.getX() - s.getX(), c.getY() - s.getY(), c.getZ());
		_solidVertices[1].xyz = core::Vector3(c.getX() - s.getX(), c.getY() + s.getY(), c.getZ());
		_solidVertices[2].xyz = core::Vector3(c.getX() + s.getX(), c.getY() + s.getY(), c.getZ());
		_solidVertices[3].xyz = core::Vector3(c.getX() + s.getX(), c.getY() - s.getY(), c.getZ());
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
}