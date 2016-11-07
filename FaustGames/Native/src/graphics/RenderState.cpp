#include "RenderState.h"
#include "Effect.h"

namespace graphics
{
	RenderState::RenderState() 
		:_effect(0, false),
		_depthState(DepthState::None, true),
		_blendState(BlendState::None, true),
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

	void RenderState::setDepthfunc(DepthFunc::e depthFunc)
	{
		_depthFunc.setState(depthFunc);
	}

	void RenderState::setVertexBuffer(GLuint vertexBufferHandle)
	{
		_vertexBufferState.setState(vertexBufferHandle);
	}

	void RenderState::applyBlend()
	{
		_blendState.applyState();

		if (!_blendState.isEqual())
		{
			switch (_blendState.getValue())
			{
			case BlendState::Alpha:
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BlendState::Additive:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BlendState::Normal:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			default:
				glDisable(GL_BLEND);
				break;
			}
			/// apply blend
		}
	}

	void RenderState::applyDepth()
	{
		_depthState.applyState();
		if (!_depthState.isEqual())
		{
			switch (_depthState.getValue())
			{
			case DepthState::None:
				glDepthMask(GL_FALSE);
				glDisable(GL_DEPTH_TEST);
				break;
			case DepthState::Read:
				glDepthMask(GL_FALSE);
				glEnable(GL_DEPTH_TEST);
				break;
			case DepthState::Write:
				glDepthMask(GL_TRUE);
				glDisable(GL_DEPTH_TEST);
				break;
			case DepthState::ReadWrite:
				glDepthMask(GL_TRUE);
				glEnable(GL_DEPTH_TEST);
				break;
			default: break;
			}
		}
	}

	void RenderState::applyDepthFunc()
	{
		_depthFunc.applyState();
		if (!_depthFunc.isEqual())
		{
			switch (_depthFunc.getValue())
			{
			case DepthFunc::Less: 
				glDepthFunc(GL_LESS);
				break;
			case DepthFunc::LessEqual: 
				glDepthFunc(GL_LEQUAL);
				break;
			default: break;
			}
		}
	}

	void RenderState::apply(VertexFormat *vertexFormat, void *vertexData)
	{
		applyDepth();
		applyBlend();
		applyDepthFunc();
		_effect.applyState();
		_vertexBufferState.applyState();
				
		
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
		_vertexFormatState.applyState();
		_vertexDataState.setState(vertexData);
		_vertexDataState.applyState();

		if (_vertexBufferState.getValue() == 0)
		{
			if (!_vertexDataState.isEqual() || !_vertexFormatState.isEqual() || !_effect.isEqual())
			{
				_effect.getValue()->applyVertexData(vertexFormat, vertexData);
			}
		}
		else
		{
			if (!_vertexFormatState.isEqual() || !_effect.isEqual())
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