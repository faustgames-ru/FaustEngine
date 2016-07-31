#include "Spaceship.h"
#include "../../game/Entity.h"
#include "../../game/Scene.h"
#include "../../core/Environment.h"
#include "../../game/input/Mouse.h"
#include "../input/InputBroker.h"

namespace razor
{
	Spaceship::Spaceship(): _mesh(nullptr)
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
		_mesh = owner->find<game::Mesh>();
	}

	void Spaceship::update(const game::UpdateArgs& e)
	{
		_velocity = core::Math::lerp(_velocity, _maxVelocity*InputBroker::Default.velocity, core::Environment::elapsedSeconds * _acceleration);
		core::Vector2 dir = (InputBroker::Default.target - owner->position.toVector2()).normalize();
		if (!dir.isEmpty())
		{
			/*
			float cross = core::Vector2::crossProduct(dir, _dir);
			if (cross > 0)
			{
				_dir = _dir.rotate(-_agularVelocity*core::Environment::elapsedSeconds);
			}
			else
			{
				_dir = _dir.rotate(_agularVelocity*core::Environment::elapsedSeconds);
			}
			*/
			_dir = dir;
			//_dir = core::Vector2::lerp(_dir, dir, core::Math::clamp(_agularVelocity*core::Environment::elapsedSeconds, 0.0f, 1.0f));
			
			dir = _dir.normalize();

			if (!_dir.isEmpty())
			{
				_mesh->transform = core::Matrix3::createRotation(core::Vector3::eZ, 0.125f, dir.getY(), dir.getX());
				owner->position += _dir*_velocity;
			}
			
		}

		e.scene->camera()->target = core::Vector3::lerp(e.scene->camera()->target, owner->position, core::Environment::elapsedSeconds * 4.0f);
		e.scene->invalidate(owner);
	}
}