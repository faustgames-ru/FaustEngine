#include "RazorGame.h"
#include <glew.h>
#include "../graphics/GraphicsDevice.h"
#include "../graphics/pipelines/RenderService.h"
#include "../graphics/pipelines/RenderVertex.h"
#include "../graphics/pipelines/UniformsConfig.h"
#include "drawing/Drawing.h"

namespace razor
{
	RazorGame::RazorGame() :
		_w(0),
		_h(0),
		_ellapsedTime(0)
	{
		_spaceShipSprite = nullptr;
		_content = new content::ContentBlock();
	}

	void RazorGame::setRootPath(String rootPath)
	{
		_content->setRoot(rootPath);
	}

	void RazorGame::updateEnvironment(int w, int h, float ellapsedTime)
	{
		_w = w;
		_h = h;
		_ellapsedTime = ellapsedTime;
	}

	void RazorGame::load()
	{
		resources::ContentManager::Default.startLoad();
		_content->enqueueResource("spaceship/blueship.frame_animation")->setLoadedCallback(this, &RazorGame::spaceShipAnimationLoaded);

		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE;
	}

	void RazorGame::update()
	{
	}

	float _time;
	core::MatrixContainer _projectionContainer;

	void RazorGame::render()
	{		
		_time += _ellapsedTime;

		_content->update();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, _w, _h);
		graphics::GraphicsDevice::Default.clear();
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
	
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;

		if (_spaceShipAnimation != nullptr)
		{
			for (int j = -1; j <= 1; j++) 
			{
				float percent = (core::Math::sin(core::Math::Pi*0.25f*j + _time * core::Math::Pi*0.5f) + 1.0f)*0.5f;
				_spaceShipSprite = _spaceShipAnimation->frameFromPercent(percent);

				SpriteTransform transform(core::Vector3(j*6.0f, 0.0f, 0.0f), 0.5f, core::Math::Pi*0.25f*j);
				Drawing::Default.drawSprite(transform, _spaceShipSprite);
			}

		}

		_projectionContainer.setValue(core::Matrix::createScale(
			1.0f / (_content->ContentScale*_w),
			1.0f / (_content->ContentScale*_h), 1.0f));
		graphics::UniformValues::projection()->setValue(_projectionContainer);
		graphics::RenderService::Default.applyPipelines();
	}

	void RazorGame::release()
	{
		delete this;
	}

	void RazorGame::spaceShipSpriteLoaded(content::ContentStatus* status)
	{
		_spaceShipSprite = status->asImage();
	}

	void RazorGame::spaceShipAnimationLoaded(content::ContentStatus* status)
	{
		_spaceShipAnimation = status->asFrameAnimation();
	}

	extern "C" DLLEXPORT IRazorGame* API_CALL createRazorGame()
	{
		return new RazorGame();
	}
}
