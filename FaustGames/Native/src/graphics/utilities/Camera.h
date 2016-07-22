#ifndef GRPAHICS_UTILITIES_CAMERA_H
#define GRPAHICS_UTILITIES_CAMERA_H

#include "../graphics_classes.h"
#include "Viewport.h"

namespace graphics
{
	class Camera2d : IBaseObject
	{
	public:
		Viewport* viewport;
		float fov;
		float scale;
		float aspect;
		float depth;
		core::Vector3 target;
		core::MatrixContainer transform;
		core::MatrixContainer skybox;
		core::MatrixContainer projection;
		core::Matrix unTransform;
		core::Matrix unProjection;
		Camera2d();
		core::Vector3 unproject(core::Vector2 screenPos, float worldZ = 0);
		core::Vector3 unproject(float windowX, float windowY, float worldZ = 0);
		float unprojectZ(float screenZ);
		float projectZ(float worldZ);
		void update();
	};
}

#endif /*GRPAHICS_UTILITIES_CAMERA_H*/