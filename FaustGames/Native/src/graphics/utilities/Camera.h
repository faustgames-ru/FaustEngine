#ifndef GRPAHICS_UTILITIES_CAMERA_H
#define GRPAHICS_UTILITIES_CAMERA_H

#include "../graphics_classes.h"

namespace graphics
{
	class Camera2d : IBaseObject
	{
	public:
		float fov;
		float scale;
		float aspect;
		float depth;
		core::Vector3 target;
		core::MatrixContainer projection;
		Camera2d();
		void update();
	};
}

#endif /*GRPAHICS_UTILITIES_CAMERA_H*/