#ifndef ZOMBO_EDITOR_CAMERA_H
#define ZOMBO_EDITOR_CAMERA_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	struct VelocityStackEntry
	{
		float time;
		core::Vector2 velocity;
		VelocityStackEntry(float t, core::Vector2 v);
	};

	class ZomboEditorCamera : public IZomboEditorCamera
	{
	public:
		static ZomboEditorCamera Default;

		float scale;
		float fov;
		float depth;
		core::Vector3 position;
		core::MatrixContainer matrix;
		
		ZomboEditorCamera();
		~ZomboEditorCamera();
		void updateInterpoaltedScale();
		void updateInput();
		void update();

		virtual void API_CALL setScale(float value) OVERRIDE;
		virtual void API_CALL setFov(float value) OVERRIDE;
		virtual float API_CALL getScale() OVERRIDE;
		virtual float API_CALL getFov() OVERRIDE;

	private:
		float _interpoaltedScale;

		float _scaleVelocity;
		core::Vector2 _lastMouse;
		core::Vector2 _velocity;
		core::Vector2 _velocityOrigin;
		float _midDownTime;
		std::queue<VelocityStackEntry> velocities;
	};
}

#endif /*ZOMBO_EDITOR_CAMERA_H*/

