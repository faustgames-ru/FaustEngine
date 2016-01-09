#ifndef ZOMBO_CAMERA_MOVE_XY_H
#define ZOMBO_CAMERA_MOVE_XY_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../ZomboEditorCamera.h"

namespace zombo
{
	class ZomboCameraMoveXY : public ZomboCameraMode
	{
	public:
		static ZomboCameraMoveXY Default;
		static std::string ModeName;
		ZomboCameraMoveXY();
		virtual void updateInput() OVERRIDE;
	private:
		core::Vector2 _lastMouse;
		core::Matrix _rotation;
		float _midDownTime;
		core::Vector2 _velocity;
		core::Vector2 _velocityOrigin;
		std::queue<VelocityStackEntry> velocities;
	};
}

#endif /*ZOMBO_CAMERA_MOVE_XY_H*/
