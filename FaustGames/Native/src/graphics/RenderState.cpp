#include "RenderState.h"
#include "Effect.h"
#include "Effects.h"

namespace graphics
{
	RenderState::RenderState() 
		:_effect(0, false),
		_depthState(DepthState::None, true),
		_blendState(BlendState::Alpha, true),
		_vertexBufferState(0, false),
		_vertexFormatState(0, false),
		_attributesState(0, 0, false),
		_attributesCount(0)
	{
	}
	
	RenderState::~RenderState()
	{
	}

	void RenderState::setEffect(EffectBase *effect)
	{
		_effect.setState(effect->getEffect());
	}
	
	void RenderState::setBlend(BlendState::e blendState)
	{
		_blendState.setState(blendState);
	}
	
	void RenderState::setDepth(DepthState::e depthState)
	{
		_depthState.setState(depthState);
	}

	void RenderState::setVertexBuffer(GLuint vertexBufferHandle)
	{
		_vertexBufferState.setState(vertexBufferHandle);
	}
		
	void RenderState::apply(VertexFormat *vertexFormat, void *vertexData)
	{
		if (!_depthState.isEqual())
		{
			/// apply depth			
		}

		//if (!_blendState.isEqual())
		{
			switch (_blendState.getValue())
			{
			case BlendState::Alpha:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BlendState::Additive:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			default:
				glDisable(GL_BLEND);
				break;
			}
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
		if (!_vertexBufferState.isEqual())
			glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferState.getValue());
		_vertexFormatState.setState(vertexFormat);
		if (_vertexBufferState.getValue() == 0)
		{
			_effect.getValue()->applyVertexData(vertexFormat, vertexData);
		}
		else
		{
			if (!_vertexFormatState.isEqual())
			{
				_effect.getValue()->applyVertexData(vertexFormat, 0);
			}
		}
	}

	void RenderState::init()
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}