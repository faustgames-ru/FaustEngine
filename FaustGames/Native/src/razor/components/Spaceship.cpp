#include "Spaceship.h"
#include "../../game/Entity.h"
#include "../../game/Scene.h"
#include "../../core/Environment.h"
#include "../../game/input/Mouse.h"
#include "../input/InputBroker.h"

namespace razor
{
	Spaceship::Spaceship(): _animation(nullptr)
	{
		updateOrder = game::UpdateOrder::Behaviors;

		_acceleration = 1.0f;
		_maxVelocity = 0.25f;
		_agularVelocity = core::Math::Pi;

		_velocity = 0;
		_dir = core::Vector2::axisY;
	}
	
	void Spaceship::loaded()
	{
		halfSize = core::Vector3::infinity;
		_animation = owner->find<game::FrameAnimation>();
		_animation->rotation = game::Rotation(1.0f, core::Math::Pi);
		_animation->setPercent(0.5f);
	}

	void Spaceship::update(const game::UpdateArgs& e)
	{
		_velocity = core::Math::lerp(_velocity, _maxVelocity*InputBroker::Default.velocity, core::Environment::elapsedSeconds * _acceleration);
		core::Vector2 dir = (InputBroker::Default.target - owner->position.toVector2()).normalize();
		if (!dir.isEmpty())
		{
			float cross = core::Vector2::crossProduct(dir, _dir);
			if (cross > 0)
			{
				_dir = _dir.rotate(-_agularVelocity*core::Environment::elapsedSeconds);
			}
			else
			{
				_dir = _dir.rotate(_agularVelocity*core::Environment::elapsedSeconds);
			}

			//_dir = core::Vector2::lerp(_dir, dir, core::Math::clamp(_agularVelocity*core::Environment::elapsedSeconds, 0.0f, 1.0f));

			dir = _dir.rotate90ccw().normalize();			
			if (!_dir.isEmpty())
			{
				_animation->rotation.c = dir.getX();
				_animation->rotation.s = dir.getY();
				owner->position += _dir*_velocity;
			}
		}

		e.scene->camera()->target = core::Vector3::lerp(e.scene->camera()->target, owner->position, core::Environment::elapsedSeconds * 4.0f);
		e.scene->invalidate(owner);
	}
}