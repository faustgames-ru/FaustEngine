#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "graphics_classes.h"
#include "VertexFormat.h"
#include "EffectBase.h"
#include "State.h"
#include "StateCompare.h"

namespace graphics
{		
	class RenderState
	{
	public:
		RenderState();
		~RenderState();
		void setEffect(EffectBase *effect);
		void setBlend(BlendState::e blendState);
		void setDepth(DepthState::e depthState);
		void setAttributesState(VertexFormat *format);
		void apply(VertexFormat *vertexFormat, void *vertexData);
	protected:
	private:
		State<Effect *> _effect;
		State<BlendState::e> _blendState;
		State<DepthState::e> _depthState;
		StateCompare<unsigned int> _attributesState;
		int _attributesCount;
	};
}

#endif /*RENDER_STATE_H*/