#ifndef RENDER_CONVERTER_H
#define RENDER_CONVERTER_H

#include "graphics_classes.h"
#include "Effects.h"
#include "VertexFormats.h"
#include "GraphicsDevice.h"

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
			_effects[llge::EffectSolidColor] = Effects::solidColor();
			_effects[llge::EffectBloomFilter] = Effects::postProcessBloomFilter();
			

			_formats[llge::FormatPositionTextureColor] = VertexFormats::positionTextureColor();
			_formats[llge::FormatPositionNormal] = VertexFormats::positionNormal();
			_formats[llge::FormatPositionTexture] = VertexFormats::positionTexture();
			_formats[llge::FormatPositionColor] = VertexFormats::positionColor();
		}
		inline EffectBase * getEffect(llge::GraphicsEffects effect)
		{
			if (GraphicsDevice::Default.config.enableFog)
			if (effect == llge::GraphicsEffects::EffectTextureColor)
			{
				return Effects::textureColorFog();
			}
			return _effects[static_cast<int>(effect)];
		}
		
		inline EffectBase * getRgbTransformeffect(llge::GraphicsEffects effect)
		{
			/*
			if (GraphicsDevice::Default.config.enableFog)
			{
			if (effect == llge::GraphicsEffects::EffectTextureColor)
			{
			return Effects::textureColorHsvFog(); //todo: use hsv
			}
			}
			if (effect == llge::GraphicsEffects::EffectTextureLightmapColor)
			{
			return Effects::textureLightmapColor(); //todo: use hsv
			}
			*/
			if (!GraphicsDevice::Default.config.useRgbTransforms)
				return getEffect(effect);
			return  Effects::textureRgbTransform();
			// _effects[static_cast<int>(effect)];
		}
		inline VertexFormat * getFormat(llge::GraphicsVertexFormats format)
		{
			return _formats[static_cast<int>(format)];
		}
		inline BlendState::e getBlend(llge::BlendMode blend)
		{
			return static_cast<BlendState::e>(blend);
		}
	private:
		EffectBase * _effects[32];
		VertexFormat * _formats[32];
	};
}

#endif /*RENDER_CONVERTER_H*/
