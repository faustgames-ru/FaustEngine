#include "FrameAnimation.h"
#include "../Entity.h"
#include "../services/Drawing.h"
#include "../../content/ContentImage.h"
#include "../../core/Environment.h"

namespace game
{
	FrameAnimation::FrameAnimation() :
		_animation(nullptr),
		_frame(nullptr),
		_frameIndex(0.0f),
		_frameRate(0.0f),
		_scale(1.0f),
		_loops(1)
	{
		updateOrder = UpdateOrder::Animations;
		renderOrder = RenderOrder::Default;
	}

	void FrameAnimation::enqueueResources(const LoadArgs& e)
	{
		e.content->enqueueResource(e.value->asString())->setLoadedCallback(this, &FrameAnimation::resourcesLoaded);
	}

	void FrameAnimation::update(const UpdateArgs& e)
	{
		int frameIndex = static_cast<int>(core::Math::trunc(_frameIndex));
		_frameIndex += _frameRate * _scale * core::Environment::elapsedSeconds;
		if(_loops > 0)
		{
			_frameIndex = core::Math::fmod(_frameIndex, _animation->frames.size()*_loops);

			int loopsCount = frameIndex / _animation->frames.size();
			if(loopsCount >= _loops)
			{
				frameIndex = _animation->frames.size()-1;
				_frameRate = 0;
			}
		}
		else
		{
			_frameIndex = core::Math::fmod(_frameIndex, _animation->frames.size());
		}
		frameIndex %= _animation->frames.size();
		_frame = _animation->frames[frameIndex];

	}

	void FrameAnimation::render()
	{
		display();
	}

	void FrameAnimation::setPercent(float percent)
	{
		_frameIndex = _animation->frames.size() * percent;
	}

	void FrameAnimation::setFrame(float frameIndex)
	{
		_frameIndex = frameIndex;
	}

	void FrameAnimation::setTime(float time)
	{
		_frameIndex = time / _animation->getFrameRate();
	}

	void FrameAnimation::play()
	{
		_frameRate = _animation->getFrameRate();
	}

	void FrameAnimation::pause()
	{
		_frameRate = 0;
	}

	void FrameAnimation::display()
	{
		SpriteTransform t;
		t.position = owner->position;
		t.rotation = rotation;
		Drawing::Default.drawSprite(t, _frame);
	}

	void FrameAnimation::resourcesLoaded(content::ContentStatus* status)
	{
		_animation = status->asFrameAnimation();
		_frame = _animation->frameFromPercent(0.0f);
		halfSize = _frame->bounds.size().toVector3();
	}
}