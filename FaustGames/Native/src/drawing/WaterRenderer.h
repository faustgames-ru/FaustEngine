#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include "drawing_classes.h"
#include "Camera.h"

namespace drawing
{
	class WaterRenderer
	{
	public:
		Camera *camera;
		core::Matrix WaterPlaneTransform;
		WaterRenderer();
		void draw();
	private:
		graphics::GraphicsDevice * _graphicsDevice;
		graphics::VertexFormat * _format;
		core::MatrixContainer _projectionContainer;
		int _drawCalls;
	};
}

#endif /*WATER_RENDERER_H*/
