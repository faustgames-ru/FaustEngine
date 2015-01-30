#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "graphics.h"

namespace graphics
{
	template<typename T>
	class State
	{
	private:
		T _value;
		bool _equal;
	public:
		inline T getValue()
		{
			return _value;
		}
		inline bool isEqual()
		{
			return _equal;
		}
		void setState(T value)
		{
			_equal = _value == value;
			_value = value;
		}
	};
	
	class RenderState
	{
	private:
		State<Effect *> _effect;
		State<BlendState::e> _blendState;
		State<DepthState::e> _depthState;
	public:
		RenderState();
		~RenderState();
		void setEffect(Effect *effect);
		void setBlend(BlendState::e blendState);
		void setDepth(DepthState::e depthState);
		void apply();
	};
}

#endif /*RENDER_STATE_H*/