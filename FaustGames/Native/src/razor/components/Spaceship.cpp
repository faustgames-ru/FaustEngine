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
	
	void Spaceship::loaded(const game::LoadedArgs& e)
	{
		halfSize = core::Vector3::infinity;
		_mesh = owner->find<game::Mesh>();
	}

	void Spaceship::update(const game::UpdateArgs& e)
	{		
		owner->position = core::Vector3::lerp(owner->position, InputBroker::Default.target.toVector3(), core::Environment::elapsedSeconds * 4.0f);

		if (!_dir.isEmpty())
		{
			_mesh->transform = core::Matrix3::createRotation(core::Vector3::eZ, 0.25f, _dir.getY(), _dir.getX());
		}

		e.scene->camera()->target = e.scene->camera()->target + core::Vector3(0, 1 * core::Environment::elapsedSeconds, 0);
		e.scene->invalidate(owner);

	}
}