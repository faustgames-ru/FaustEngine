#include "RazorGame.h"
#include <glew.h>
#include "../graphics/GraphicsDevice.h"
#include "../graphics/pipelines/RenderService.h"
#include "../graphics/pipelines/RenderVertex.h"
#include "../graphics/pipelines/UniformsConfig.h"
#include "drawing/Drawing.h"
#include "../core/Environment.h"

namespace razor
{
	RazorGame::RazorGame(): _tree(geometry::Aabb(-1000, -1000, -10, 1000, 1000, 20), 0)
	{
		_spaceShipAnimation = nullptr;
		_spaceShipSprite = nullptr;
		_content = new content::ContentBlock();
	}

	void RazorGame::setRootPath(String rootPath)
	{
		_content->setRoot(rootPath);
	}

	void RazorGame::updateEnvironment(int w, int h, float ellapsedTime)
	{
		_viewport.update(w, h);
		core::Environment::update(ellapsedTime);
	}

	void RazorGame::load()
	{
		resources::ContentManager::Default.startLoad();
		_content->enqueueResource("spaceship/blueship.frame_animation")->setLoadedCallback(this, &RazorGame::spaceShipAnimationLoaded);

		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE
	}

	void RazorGame::update()
	{
		_camera.scale = 4.0f;
		_camera.aspect = _viewport.getAspect();
		_camera.update();
	}

	float _time = 0;

	void RazorGame::render()
	{		
		_content->update();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, _viewport.w, _viewport.h);
		graphics::GraphicsDevice::Default.clear();
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
	
		graphics::RenderPipeline* pipeline = graphics::RenderService::Default.pipeline;
		
		if (_spaceShipAnimation != nullptr)
		{
			for (int j = -1; j <= 1; j++) 
			{
				float percent = (core::Math::sin(core::Math::Pi*0.25f*j + _time * core::Math::Pi*2.0f) + 1.0f)*0.5f;
				_spaceShipSprite = _spaceShipAnimation->frameFromPercent(percent);

				SpriteTransform transform(core::Vector3(j*6.0f, 0.0f, 0.0f), 0.5f, core::Math::Pi*0.25f*j);
				Drawing::Default.drawSprite(transform, _spaceShipSprite);
			}
		}		

		geometry::Frustum frustum(_camera.projection.Value);
		_tree.foreachLeaf(&frustum, this, &RazorGame::drawQuadTreeLeaf);
		graphics::UniformValues::projection()->setValue(_camera.projection);
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

	void RazorGame::drawQuadTreeNode(geometry::QuadTreeNode* node)
	{
		Drawing::Default.drawAabb(node->getAabb());
	}

	void RazorGame::drawQuadTreeLeaf(geometry::QuadTreeLeaf* leaf)
	{
		Drawing::Default.drawAabb(leaf->aabb);
	}

	extern "C" DLLEXPORT IRazorGame* API_CALL createRazorGame()
	{
		return new RazorGame();
	}
}
