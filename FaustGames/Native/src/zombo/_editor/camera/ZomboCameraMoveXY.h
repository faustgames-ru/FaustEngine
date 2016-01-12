#ifndef ZOMBO_CAMERA_MOVE_XY_H
#define ZOMBO_CAMERA_MOVE_XY_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../ZomboEditorCamera.h"
#include "../commands/ZomboEditorCommand.h"

namespace zombo
{
	class ZomboCommandCameraMoveXY: public ZomboEditorCommand
	{
	public:
		ZomboCommandCameraMoveXY(const core::Vector2 prevPosition, const core::Vector2 newPosition);
		void invalidate(const core::Vector2 newPosition);

		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	private:
		core::Vector2 _prevPosition;
		core::Vector2 _targetPosition;
	};

	class ZomboCameraMoveXY : public ZomboCameraMode
	{
	public:
		static ZomboCameraMoveXY Default;
		static std::string ModeName;
		ZomboCameraMoveXY();
		virtual void activated() OVERRIDE;
		virtual void updateInput() OVERRIDE;
	private:
		core::Vector2 _lastMouse;
		core::Matrix _rotation;
		ZomboCameraVelocityStack velocityStack;
		ZomboInterpolatedValue _pAngle;
		ZomboInterpolatedValue _hAngle;
		ZomboInterpolatedValue _bAngle;
		ZomboCommandCameraMoveXY * _actualCommand;
		core::Vector2 _prevPosition;
		bool _prevMidButtonState;
	};
}

#endif /*ZOMBO_CAMERA_MOVE_XY_H*/
