#include "Spaceship.h"
#include "../../game/Entity.h"
#include "../../game/Scene.h"
#include "../../core/Environment.h"

namespace razor
{
	Spaceship::Spaceship(): _animation(nullptr)
	{
		updateOrder = game::UpdateOrder::Behaviors;
	}

	void Spaceship::loaded()
	{
		halfSize.setData(1, 1, 0);
		_animation = owner->find<game::FrameAnimation>();
		_animation->rotation = game::Rotation(1.0f, core::Math::Pi);
		_animation->setPercent(0.5f);
	}

	void Spaceship::update(const game::UpdateArgs& e)
	{
		//float period = 4.0f;
		//ulong delta = core::Environment::gameTime % core::Environment::getTime(period);
		//float percent = core::Environment::getSeconds(delta) / period;
		//float offest = core::Math::abs(percent - 0.5f)*2.0f;
		//_animation->setPercent(offest);
		//owner->position.setZ((offest-0.5f) * 10);
		//
		//e.scene->invalidate(_animation);
		owner->position = owner->position + core::Vector3(0.0f, 5.0f* core::Environment::elapsedSeconds, 0.0f);		
		e.scene->camera()->target = owner->position;
		
		e.scene->invalidate(_animation);
		e.scene->invalidate(this);
	}
}