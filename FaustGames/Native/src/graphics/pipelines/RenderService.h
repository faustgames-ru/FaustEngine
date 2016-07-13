#ifndef GRPAHICS_PIPELINES_RENDER_SERVICE_H
#define GRPAHICS_PIPELINES_RENDER_SERVICE_H

#include "../graphics_classes.h"
#include "RenderPipeline.h"

namespace graphics
{
	class RenderService
	{
	public:
		static RenderService Default;
		RenderPipeline* pipeline;
		RenderService();
		void applyPipelines();
	private:
		std::vector<RenderPipeline* > _pipelines;
	};
}

#endif