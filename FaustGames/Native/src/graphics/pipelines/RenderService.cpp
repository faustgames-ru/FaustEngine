#include "RenderService.h"

namespace graphics
{
	RenderService RenderService::Default;

	RenderService::RenderService()
	{
		pipeline = new RenderPipeline();
	}

	void RenderService::applyPipelines()
	{
		pipeline->apply();
	}
}
