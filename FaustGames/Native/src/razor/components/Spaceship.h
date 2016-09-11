#ifndef RAZOR_COMPONENTS_SPACESHIP_H
#define RAZOR_COMPONENTS_SPACESHIP_H

#include "../razor_classes.h"
#include "../../content/ContentFrameAnimation.h"
#include "../../game/Component.h"
#include "../../game/components/FrameAnimation.h"
#include "../../game/components/Mesh.h"

namespace razor
{
	class Spaceship : public game::Component
	{
	public:
		Spaceship();
		virtual void loaded(const game::LoadedArgs& e) OVERRIDE;
		virtual void update(const game::UpdateArgs& e) OVERRIDE;
	private:
		game::Mesh* _mesh;
		float _acceleration;
		float _maxVelocity;
		float _velocity;
		float _agularVelocity;
		core::Vector2 _dir;

	};
}


#endif /*RAZOR_COMPONENTS_SPACESHIP_H*/