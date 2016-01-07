#ifndef ZOMBO_CAMERA_ROTATE_H
#define ZOMBO_CAMERA_ROTATE_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../ZomboEditorCamera.h"

namespace zombo
{

	class ZomboCameraRotate : public ZomboCameraMode
	{
	public:
		static ZomboCameraRotate Default;
		ZomboCameraRotate();
		virtual void updateInput() OVERRIDE;
	private:
		core::Vector2 _mouseDownPosition;
		core::Matrix _originRotation;
	};
}

#endif /*ZOMBO_CAMERA_ROTATE_H*/