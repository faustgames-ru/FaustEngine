#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "graphics.h"
#include "VertexFormat.h"

namespace graphics
{
	template<typename T>
	class State
	{
	private:
		T _value;
		bool _equal;
	public:
		State(){}
		State(T value, bool equal)
		{
			_value = value;
			_equal = equal;
		}
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
	
	template<typename T>
	class StateCompare
	{
	private:
		T _prevValue;
		T _value;
		bool _equal;
	public:
		StateCompare(){}
		StateCompare(T value, T prevVaolue, bool equal)
		{
			_value = value;
			_prevValue = prevVaolue;
			_equal = equal;
		}
		inline T getPrevValue()
		{
			return _prevValue;
		}
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
			_prevValue = _value;
			_value = value;
		}
	};


	class RenderState
	{
	private:
		State<Effect *> _effect;
		State<BlendState::e> _blendState;
		State<DepthState::e> _depthState;
		StateCompare<unsigned int> _attributesState;
		int _attributesCount;
	public:
		RenderState();
		~RenderState();
		void setEffect(Effect *effect);
		void setBlend(BlendState::e blendState);
		void setDepth(DepthState::e depthState);
		void setAttributesState(VertexFormat *format);
		void apply(VertexFormat *vertexFormat, void *vertexData);
	};
}

#endif /*RENDER_STATE_H*/