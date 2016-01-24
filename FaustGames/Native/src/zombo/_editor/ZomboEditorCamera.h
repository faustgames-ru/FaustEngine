#ifndef ZOMBO_EDITOR_CAMERA_H
#define ZOMBO_EDITOR_CAMERA_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../common/ZomboInterpolatedValue.h"
#include "commands/ZomboEditorCommand.h"

namespace zombo
{	
	class ZomboCommandCameraSetMode : public ZomboEditorCommand
	{
	public:
		std::string originMode;
		std::string targetMode;
		ZomboCommandCameraSetMode(const std::string &mode);
		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	private:
		core::Matrix _prevRotation;
	};

	class ZomboCommandCameraSetFov : public ZomboEditorCommand
	{
	public:
		ZomboCommandCameraSetFov(float fov);
		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	private:
		float _fov;
		float _prevFov;
	};

	class ZomboCommandCameraSetScale : public ZomboEditorCommand
	{
	public:
		ZomboCommandCameraSetScale(float scale);
		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
		void invalidate(float value);
	private:
		float _scale;
		float _prevscale;
	};

	struct ZomboCameraVelocityEntry
	{
		ulong time;
		core::Vector2 velocity;
		ZomboCameraVelocityEntry(ulong t, core::Vector2 v);
	};

	class ZomboCameraVelocityStack
	{
	public:
		ZomboCameraVelocityStack();
		typedef std::list<ZomboCameraVelocityEntry> Velocities;
		void updateMove(const core::Vector2 &v);
		core::Vector2 updateVelocity();
	private:
		float _resistance;
		core::Vector2 _velocity;
		core::Vector2 _velocityOrigin;
		Velocities _velocities;
	};

	class ZomboCameraMode : IBaseObject
	{
	public:
		virtual void activated() = 0;
		virtual void updateInput() = 0;
	};
	
	class ZomboEditorCameraRotator
	{
	public:
		ZomboEditorCameraRotator();
		void setRotation(const core::Matrix &matrix);
		core::Matrix originRotation;
		core::Matrix actualRotation;
		core::Matrix targetRotation;
		void ternimateInterpolator();
		void rotate(core::Matrix originMatrix, core::Vector3 n, float a, core::Matrix targetMatrix);
		void setRotationAnimated(const core::Matrix &matrix);
		void updateEuler();
		void update();
	private:
		core::Vector3 _normal;
		ZomboInterpolatedValue _angle;
		ZomboInterpolatedValue _pAngle;
		ZomboInterpolatedValue _hAngle;
		ZomboInterpolatedValue _bAngle;
	};

	class ZomboEditorCamera : public IZomboEditorCamera
	{
	public:
		static ZomboEditorCamera Default;
		
		ZomboCameraMode * mode;

		float depth;

		core::MatrixContainer matrix;
		core::MatrixContainer skyboxMatrix;
		ZomboEditorCameraRotator rotator;

		core::Matrix transformToView;
		core::Matrix transformFromView;

		ZomboEditorCamera();
		~ZomboEditorCamera();
		void updateInterpoaltedScale();
		void update();

		core::Vector2 getMouseProjection(float z) const;

		virtual IntPtr API_CALL getMode() OVERRIDE;
		virtual void API_CALL setMode(String modeName) OVERRIDE;
		virtual void API_CALL setScale(float value) OVERRIDE;
		virtual void API_CALL setFov(float value) OVERRIDE;
		virtual float API_CALL getScale() OVERRIDE;
		virtual float API_CALL getFov() OVERRIDE;
		virtual bool API_CALL isUndoAvaliable() OVERRIDE;
		virtual bool API_CALL isRedoAvaliable() OVERRIDE;
		virtual void API_CALL undo() OVERRIDE;
		virtual void API_CALL redo() OVERRIDE;

		void setModeInternal(String mode);
		float getInterpoaltedScale() const;
		void setPositionX(float x);
		void setPositionY(float y);
		float getPositionX() const;
		float getPositionY() const;
		void addPositionXY(const core::Vector2 &d);
		void setPositionXY(const core::Vector2 &v);
		core::Vector2 getPositionXY() const;
		void setFovInternal(float value);
		void setScaleInternal(float value);
		std::string getModeInternal();
	private:
		std::string _actualModeName;
		ZomboInterpolatedValue _scaleValue;
		ZomboInterpolatedValue _fovValue;
		ZomboInterpolatedValue _positionX;
		ZomboInterpolatedValue _positionY;
		ZomboInterpolatedValue _positionZ;
		ZomboCommandCameraSetScale *_lastZoomCommand;
	};
}

#endif /*ZOMBO_EDITOR_CAMERA_H*/

