#ifndef ZOMBO_EDITOR_CAMERA_H
#define ZOMBO_EDITOR_CAMERA_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
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
		float _lastMouseX;
		float _lastMouseY;
		float _velocityX;
		float _velocityY;
		float _velocityOriginX;
		float _velocityOriginY;
	};
}

#endif /*ZOMBO_EDITOR_CAMERA_H*/

