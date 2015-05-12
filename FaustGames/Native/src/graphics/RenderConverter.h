#ifndef RENDER_CONVERTER_H
#define RENDER_CONVERTER_H

#include "graphics_classes.h"
#include "Effects.h"
#include "VertexFormats.h"

namespace graphics
{
	class RenderConverter
	{
	public:
		RenderConverter()
		{
			_effects[llge::EffectTextureColor] = Effects::textureColor();
			_effects[llge::EffectTextureLightmapColor] = Effects::textureLightmapColor();
			_effects[llge::EffectWater] = 0;// Effects::water();
			_effects[llge::EffectSolid] = Effects::solid();
			_effects[llge::EffectRenderDepth] = Effects::renderDepth();
			_effects[llge::EffectSolidColor] = Effects::solidColor();

			_formats[llge::FormatPositionTextureColor] = VertexFormats::positionTextureColor();
			_formats[llge::FormatPositionNormal] = VertexFormats::positionNormal();
			_formats[llge::FormatPositionTexture] = VertexFormats::positionTexture();
			_formats[llge::FormatPositionColor] = VertexFormats::positionColor();
		}
		inline EffectBase * getEffect(llge::GraphicsEffects effect)
		{
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

#endif /*RENDER_CONVERTER_H*/
