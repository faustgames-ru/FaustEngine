#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "graphics_classes.h"
#include "VertexFormat.h"
#include "EffectBase.h"
#include "State.h"
#include "StateCompare.h"
#include "VertexBuffer.h"

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
		void setVertexBuffer(GLuint vertexBufferHandle);
		void apply(VertexFormat *vertexFormat, void *vertexData);
		void init();

	protected:
	private:
		State<GLuint> _vertexBufferState;
		State<VertexFormat *> _vertexFormatState;
		State<Effect *> _effect;
		State<void *> _vertexDataState;
		State<BlendState::e> _blendState;
		State<DepthState::e> _depthState;
		StateCompare<unsigned int> _attributesState;
		int _attributesCount;
	};
}

#endif /*RENDER_STATE_H*/