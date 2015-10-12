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
		static RenderConverter * lastCreatedInstance;
		static RenderConverter * getInstance() 
		{
			if (!lastCreatedInstance)
				lastCreatedInstance = new RenderConverter();
			return lastCreatedInstance;
		}
		RenderConverter()
		{
			lastCreatedInstance = this;
			_effects[llge::EffectTextureColor] = Effects::textureColor();
			_effects[llge::EffectTextureColorHighlight] = Effects::textureColorHighlight();
			_effects[llge::EffectTextureAlphamaskColor] = Effects::textureAlphamaskColor();			
			_effects[llge::EffectTextureLightmapColor] = Effects::textureLightmapColor();
			_effects[llge::EffectWater] = 0;// Effects::water();
			_effects[llge::EffectSolid] = Effects::solid();
			_effects[llge::EffectRenderDepth] = Effects::renderDepth();
			_effects[llge::EffectSolidColor] = Effects::solidColor();
			_effects[llge::EffectBloomFilter] = Effects::postProcessBloomFilter();
			

			_formats[llge::FormatPositionTextureColor] = VertexFormats::positionTextureColor();
			_formats[llge::FormatPositionNormal] = VertexFormats::positionNormal();
			_formats[llge::FormatPositionTexture] = VertexFormats::positionTexture();
			_formats[llge::FormatPositionColor] = VertexFormats::positionColor();
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

#endif /*RENDER_CONVERTER_H*/
