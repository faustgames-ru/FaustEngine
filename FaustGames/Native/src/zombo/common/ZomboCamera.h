#ifndef ZOMBO_CAMERA_H
#define ZOMBO_CAMERA_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboCamera
	{
	public:
		float fov;
		float scale;
		float aspect;
		float nearPlane;
		float farPlane;

		static ZomboCamera* create();
		ZomboCamera();
		~ZomboCamera();
		void dispose() const;
		
		bool areBoundsVisible(const core::Vector3 &position, const core::Vector2 &size);
	};
}

#endif /* ZOMBO_CAMERA_H */