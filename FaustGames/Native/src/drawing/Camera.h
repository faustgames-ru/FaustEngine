#ifndef CAMERA_H
#define CAMERA_H

#include "drawing_classes.h"

namespace drawing
{
	class Camera
	{
	public:
		Camera();
		void setPosition(float x, float y, float z);
		void setFov(float fov);
		void setAspect(float aspect);
		void setAspect(float w, float h);
		void setRotation(float rotationZ);
		void setPlanes(float zn, float zf);
		core::Matrix getMatrix();
	private:
		core::Matrix getProjectionMatrix();
		core::Matrix getTranslationMatrix();
		core::Vector3 _position;
		float _fov;
		float _rotationZ;
		float _aspect;
		float _zn;
		float _zf;
	};
}

#endif /*CAMERA_H*/
