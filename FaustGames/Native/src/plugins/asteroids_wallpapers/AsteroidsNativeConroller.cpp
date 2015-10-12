#include "asteroids.h"
#include "../../graphics/graphics.h"
#include "../../resources/resources.h"

namespace asteroids
{
	struct RenderVertex
	{
		core::Vector3 xyz;
		core::Vector2 uv;
		uint color;
		RenderVertex(float x, float y, float z, float u, float v, uint c)
		{
			xyz = core::Vector3(x, y, z);
			uv = core::Vector2(u, v);
			color = c;
		}
	};

	class AsteroidsNativeController : public IAsteroidsNativeController
	{
	private:
		static RenderVertex _vertices[];
		static ushort _indices[];
		static core::MatrixContainer _projection;
		static graphics::TextureImage2d _image;
	public:
		static AsteroidsNativeController Default;
		virtual void API_CALL load();
		virtual void API_CALL update(float delta);
		virtual void API_CALL render(float w, float h);
		virtual void API_CALL dispose();
	};

	AsteroidsNativeController AsteroidsNativeController::Default;
	graphics::TextureImage2d AsteroidsNativeController::_image(false, true);


	RenderVertex AsteroidsNativeController::_vertices[] =
	{
		RenderVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0xffffffff),
		RenderVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0xffffffff),
		RenderVertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0xffffffff),
		RenderVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0xffffffff),
	};

	ushort AsteroidsNativeController::_indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
	
	core::MatrixContainer  AsteroidsNativeController::_projection(core::Matrix::identity);

	extern "C" DLLEXPORT IAsteroidsNativeController * API_CALL createAsteroidsNativeController()
	{
		return &AsteroidsNativeController::Default;
	}

	void API_CALL AsteroidsNativeController::load()
	{
		graphics::GraphicsDevice::Default.renderState = graphics::RenderState(); /// ???
		graphics::GraphicsDevice::Default.renderState.init();

		graphics::GraphicsDevice::create();
		graphics::Effects::create();
		graphics::VertexFormats::create();

		graphics::GraphicsDevice::Default.setClearState(0xff000000, 1.0f);

		_image.create();
		/*
		resources::ContentManager::Default.open();
		int test_texture_id = resources::ContentManager::Default.registerImage("test_texture.png");
		resources::ContentManager::Default.loadImage(test_texture_id, &_image);
		resources::ContentManager::Default.close();
		*/
	}
	
	void API_CALL AsteroidsNativeController::update(float delta)
	{
		//resources::ContentManager::Default.update();
	}
	
	void API_CALL AsteroidsNativeController::render(float w, float h)
	{
		graphics::GraphicsDevice::Default.clear();
		
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);

		graphics::GraphicsDevice::Default.setViewport(0, 0, w, h);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::solidColor());

		graphics::UniformValues::projection()->setValue(_projection);
		//graphics::UniformValues::texture()->setValue(_image.getId());

		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTextureColor(), _vertices, _indices, 2);
		
	}
	
	void API_CALL AsteroidsNativeController::dispose()
	{
		delete this;
	}

	
#if __ANDROID__


#include <jni.h>

	extern "C"
	JNIEXPORT void JNICALL
	Java_llge_AsteroidsNativeController_load(JNIEnv* env, jobject thiz)
	{
		AsteroidsNativeController::Default.load();
	}

	extern "C"
	JNIEXPORT void JNICALL
	Java_llge_AsteroidsNativeController_update(JNIEnv* env, jobject thiz, jfloat delta)
	{
		AsteroidsNativeController::Default.update((float)delta);
	}

	extern "C"
	JNIEXPORT void JNICALL
	Java_llge_AsteroidsNativeController_render(JNIEnv* env, jobject thiz, jfloat w, jfloat h)
	{
		AsteroidsNativeController::Default.render((float)w, (float)h);
	}
#endif
}
