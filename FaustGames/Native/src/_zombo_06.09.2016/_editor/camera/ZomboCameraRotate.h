#ifndef ZOMBO_CAMERA_ROTATE_H
#define ZOMBO_CAMERA_ROTATE_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../ZomboEditorCamera.h"

namespace zombo
{

	class CameraRotationAngles
	{
	public:
		float pAngle;
		float hAngle;
		float bAngle;
	};

	class ZomboCommandCameraRotate : public ZomboEditorCommand
	{
	public:
		ZomboCommandCameraRotate(core::Vector3 n, float a, const core::Matrix &prev, const core::Matrix &target);
		
		void invalidate(core::Vector3 n, float a, const core::Matrix &target);
		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	private:
		core::Matrix _prev;
		core::Matrix _target;
		core::Vector3 _normal;
		float _angle;
	};


	class ZomboCameraRotate : public ZomboCameraMode
	{
	public:
		static ZomboCameraRotate Default;
		static std::string ModeName;
		ZomboCameraRotate();
		virtual void activated() OVERRIDE;
		virtual void updateInput() OVERRIDE;
	private:
		core::Vector2 _mouseDownPosition;
		core::Vector2 _lastMouse;
		core::Matrix _originRotation;
		ZomboCameraVelocityStack _velocityStack;
		ZomboCommandCameraRotate * _actualCommand;
		bool _prevMidButtonState;
		core::Vector3 _n;
		float _angle;
	};
}

#endif /*ZOMBO_CAMERA_ROTATE_H*/