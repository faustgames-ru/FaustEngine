#ifndef RENDER_CONVERTER_BASIC_H
#define RENDER_CONVERTER_BASIC_H

#include "graphics_classes.h"
#include "EffectsBasic.h"
#include "VertexFormatsBasic.h"

namespace graphics
{
	class RenderConverterBasic
	{
	public:
		static RenderConverterBasic * lastCreatedInstance;
		static RenderConverterBasic * getInstance()
		{
			if (!lastCreatedInstance)
				lastCreatedInstance = new RenderConverterBasic();
			return lastCreatedInstance;
		}
		RenderConverterBasic()
		{
			lastCreatedInstance = this;
			_effects[llge::EffectSolidColor] = EffectsBasic::solidColor();
			_effects[llge::EffectTextureColor] = EffectsBasic::textureColor();
		

			_formats[llge::FormatPositionTextureColor] = VertexFormatsBasic::positionColorTexture();
			_formats[llge::FormatPositionColor] = VertexFormatsBasic::positionColor();
		}
		inline EffectBase * getEffect(llge::GraphicsEffects effect)
		{
			if (effect == llge::GraphicsEffects::EffectTextureColor)
			{
				effect = llge::GraphicsEffects::EffectTextureColor;
			}
			return _effects[(int)effect];
		}
		inline VertexFormat * getFormat(llge::GraphicsVertexFormats format)
		{
			return _formats[(int)format];
		}
		inline BlendState::e getBlend(llge::BlendMode blend)
		{
			return (BlendState::e)blend;
		}
	private:
		EffectBase * _effects[32];
		VertexFormat * _formats[32];
	};
}

#endif /*RENDER_CONVERTER_BASIC_H*/
