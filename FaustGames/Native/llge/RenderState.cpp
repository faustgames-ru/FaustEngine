#include "stdafx.h"
#include "RenderState.h"
#include "Effect.h"
#include "Effects.h"

namespace graphics
{
	RenderState::RenderState()
	{
		setEffect(Effects::instance()->solid()->getEffect());
		setBlend(BlendState::Alpha);
		setDepth(DepthState::None);
	}
	
	RenderState::~RenderState()
	{
	}

	void RenderState::setEffect(Effect *effect)
	{
		_effect.setState(effect);
	}
	
	void RenderState::setBlend(BlendState::e blendState)
	{
		_blendState.setState(blendState);
	}
	
	void RenderState::setDepth(DepthState::e depthState)
	{
		_depthState.setState(depthState);
	}
	
	void RenderState::apply()
	{
		if (!_depthState.isEqual())
		{
			/// apply depth			
		}

		if (!_blendState.isEqual())
		{
			/// apply blend
		}

		if (!_effect.isEqual())
			_effect.getValue()->applyShader();
		_effect.getValue()->applyUniforms();
	}
}