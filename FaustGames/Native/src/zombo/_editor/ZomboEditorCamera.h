#ifndef ZOMBO_EDITOR_CAMERA_H
#define ZOMBO_EDITOR_CAMERA_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../common/ZomboInterpolatedValue.h"

namespace zombo
{	
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
	
	class ZomboEditorCamera : public IZomboEditorCamera
	{
	public:
		static ZomboEditorCamera Default;
		
		ZomboCameraMode * mode;

		float depth;
		
		core::MatrixContainer matrix;
		core::Matrix rotation;

		ZomboEditorCamera();
		~ZomboEditorCamera();
		void updateInterpoaltedScale();
		void update();


		virtual IntPtr API_CALL getMode() OVERRIDE;
		virtual void API_CALL setMode(String modeName) OVERRIDE;
		virtual void API_CALL setScale(float value) OVERRIDE;
		virtual void API_CALL setFov(float value) OVERRIDE;
		virtual float API_CALL getScale() OVERRIDE;
		virtual float API_CALL getFov() OVERRIDE;
		void setEditorModeInternal(String mode);
		float getInterpoaltedScale() const;
		void setPositionX(float x);
		void setPositionY(float y);
		float getPositionX() const;
		float getPositionY() const;
		void addPositionXY(const core::Vector2 &d);
		void setPositionXY(const core::Vector2 &v);
		core::Vector2 getPositionXY() const;
	private:
		std::string _actualModeName;
		ZomboInterpolatedValue _scaleValue;
		ZomboInterpolatedValue _fovValue;
		ZomboInterpolatedValue _positionX;
		ZomboInterpolatedValue _positionY;
		ZomboInterpolatedValue _positionZ;
	};
}

#endif /*ZOMBO_EDITOR_CAMERA_H*/

