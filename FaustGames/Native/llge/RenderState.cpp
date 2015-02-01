#include "stdafx.h"
#include "RenderState.h"
#include "Effect.h"
#include "Effects.h"

namespace graphics
{
	RenderState::RenderState() 
		:_effect(0, false),
		_depthState(DepthState::None, true),
		_blendState(BlendState::Alpha, true),
		_attributesState(0, 0, false),
		_attributesCount(0)
	{
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
	
	void RenderState::setAttributesState(VertexFormat *format)
	{
	}

	void RenderState::apply(VertexFormat *vertexFormat, void *vertexData)
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
		{
			_effect.getValue()->applyShader();
			_attributesState.setState(_effect.getValue()->getAttributesMask());
			if (!_attributesState.isEqual())
			{
				int max = _effect.getValue()->getAttributesMax();
				if (_attributesCount < max)
					_attributesCount = max;
				Effect::applyState(_attributesState.getPrevValue(), _attributesState.getValue(), _attributesCount);
			}
		}
		_effect.getValue()->applyUniforms();
		_effect.getValue()->applyVertexData(vertexFormat, vertexData);

	}
}