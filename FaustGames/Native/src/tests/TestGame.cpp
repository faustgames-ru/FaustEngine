#include "TestGame.h"
#include "../resources/ContentManager.h"

namespace tests
{
	TestGame::TestGame()
	{
		_batcher = new drawing::Batcher();
	}

	void TestGame::setRootPath(String rootPath)
	{
		_root = rootPath;
	}

	int swapRb(int c)
	{
		return graphics::Color::fromRgba(
			graphics::Color::getB(c),
			graphics::Color::getG(c),
			graphics::Color::getR(c),
			graphics::Color::getA(c));
	}

	void TestGame::updateColors(IntPtr values)
	{
		int* colors = static_cast<int*>(values);
		int hairColor = swapRb(colors[0]);
		int armsColor = swapRb(colors[1]);
		int thighsColor = swapRb(colors[2]);
		int handColor = swapRb(colors[3]);
		int footsColor = swapRb(colors[4]);
		int weaponColor = swapRb(colors[5]);
		_skeleton->setSlotRgb("head", hairColor);

		_skeleton->setSlotRgb("upperarm1", armsColor);
		_skeleton->setSlotRgb("upperarm2", armsColor);
		_skeleton->setSlotRgb("hand1", handColor);
		_skeleton->setSlotRgb("hand2", handColor);
		_skeleton->setSlotRgb("fingers", handColor);

		_skeleton->setSlotRgb("thigh1", thighsColor);
		_skeleton->setSlotRgb("thigh2", thighsColor);
		_skeleton->setSlotRgb("foot1", footsColor);
		_skeleton->setSlotRgb("foot2", footsColor);

		_skeleton->setSlotRgb("weapon", weaponColor);
	}

	void TestGame::updateMouse(int rawX, int rawY, int windowX, int windowY, uint buttons)
	{
	}

	void TestGame::updateEnvironment(int w, int h, float ellapsedTime)
	{
		_w = w;
		_h = h;
		_t = ellapsedTime;
	}

	void TestGame::load()
	{
		resources::ContentManager::Default.startLoad();
		graphics::GraphicsDevice::Default.config.postEffectsScale = 1.0f;
		graphics::GraphicsDevice::Default.config.bloomDownsample = 1;
		graphics::GraphicsDevice::Default.config.earlyFragmentTestsShaderCode = false;
		graphics::GraphicsDevice::Default.config.generateMipmaps = false;
		graphics::GraphicsDevice::Default.config.mipmapsLevel = 0;
		graphics::GraphicsDevice::Default.config.earlyDepthPath = true;
		graphics::GraphicsDevice::Default.config.enableFog = false;
		graphics::GraphicsDevice::Default.config.filterMode = graphics::FilterMode::Linear;

		graphics::GraphicsDevice::Default.config.refraction = false;
		graphics::GraphicsDevice::Default.config.vignetting = false;
		graphics::GraphicsDevice::Default.config.ellapsedTime = 0.02f;
		graphics::GraphicsDevice::Default.config.postEffectsScale = 1.0f;


		std::string atlas = resources::ContentManager::Default.loadString((_root + "hero-mesh.atlas").c_str());
		std::string skeleton = resources::ContentManager::Default.loadString((_root + "hero-mesh.json").c_str());


		_skeletonResource = new spine::SpineSkeletonResource();
		_skeletonResource->load(atlas.c_str(), skeleton.c_str(), _root.c_str());
		_skeleton = new spine::SpineSkeleton(_skeletonResource, reinterpret_cast<float *>(&core::Matrix::identity));
		_walk = _skeletonResource->findAnimation("Walk");
		_animationStateData = new spine::SpineAnimationStateData(_skeletonResource);
		_animationState = new spine::SpineAnimationState(_animationStateData);
		_animationState->setAnimation(_walk, true, false);
		_paintmask = resources::ContentManager::Default.addLoadTexture((_root + "hero_mask.png").c_str());

		int colors[] = {
			0xff00ff00,
			0xff0000ff,
			0xffffff00,
			0xffff0000,
			0xff00ffff,
			0xffff00ff 
		};

		updateColors(colors);	

		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE
	}

	void TestGame::update()
	{

	}

	void TestGame::render()
	{
		resources::ContentManager::Default.update();

		
		_animationState->update(core::Math::min(_t, 0.03));
		_animationState->apply(_skeleton);
		_skeleton->updateWorldTransform();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, _w, _h);
		graphics::GraphicsDevice::Default.clear();
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);

		float aspect = _w / _h;
		float scale = 0.00125f;
		core::Matrix proj = core::Matrix::mul(core::Matrix::createScale(scale, scale*aspect, 1.0f), core::Matrix::createTranslate(0, -0.5f, 0.0f));

		llge::LightingConfig _lightingConfig;
		_lightingConfig.lightmap = _paintmask->getHandle();
		_lightingConfig.texture = 0;
		
		_batcher->start();
		_batcher->addProjection(&proj);
		_skeleton->renderEx(_batcher, &_lightingConfig, llge::EffectTexturePaint, 128);
		_batcher->finish();
		_batcher->executeRenderCommands(false);
	}

	void TestGame::release()
	{
		delete this;
	}
}

llge::ITestGame* __stdcall llge::createTestGame()
{
	return new tests::TestGame();
}
