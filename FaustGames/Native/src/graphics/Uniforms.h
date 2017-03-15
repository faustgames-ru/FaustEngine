#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "graphics_classes.h"
#include "UniformInfo.h"
#include "UniformValueFloat.h"
#include "UniformValueVector3.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"
#include "UniformValueCubemap.h"

namespace graphics
{
	class Uniforms
	{
	public:
		static UniformInfo *cameraPosition();
		static UniformInfo *projection();
		static UniformInfo *colorTransform();
		static UniformInfo *texture();
		static UniformInfo *lightmap();
		static UniformInfo *environment();
		static UniformInfo *depthmap();
		static UniformInfo *normalmap();
		static UniformInfo *time();
		static UniformInfo *pixelSize();
		static UniformInfo *pixelOffset();
		static UniformInfo *highlightColor();
		static UniformInfo *fogStart();
		static UniformInfo *fogDensity();
		static UniformInfo *fogColor();
		static UniformInfo *fogScale();
		static UniformInfo *paintmask();
	private:
		static UniformInfo _cameraPosition;
		static UniformInfo _projection;
		static UniformInfo _colorTransform;
		static UniformInfo _texture;
		static UniformInfo _lightmap;
		static UniformInfo _environment;
		static UniformInfo _depthmap;
		static UniformInfo _normalmap;
		static UniformInfo _time;
		static UniformInfo _pixelSize;
		static UniformInfo _pixelOffset;
		static UniformInfo _highlightColor;
		static UniformInfo _paintmask;
		static UniformInfo _fogStart;
		static UniformInfo _fogDensity;
		static UniformInfo _fogScale;
		static UniformInfo _fogColor;
	};

	class UniformValues
	{
	public:
		static inline UniformValueVector3 * cameraPosition(){ return &_cameraPosition; }
		static inline UniformValueMatrix *projection(){ return &_projection; }
		static inline UniformValueMatrix3 *colorTransform() { return &_colorTransform; }
		static inline UniformValueTexture *texture(){ return &_texture; }
		static inline UniformValueTexture *lightmap(){ return &_lightmap; }
		static inline UniformValueTexture *environment(){ return &_environment; }
		static inline UniformValueTexture *depthmap(){ return &_depthmap; }
		static inline UniformValueTexture *normalmap(){ return &_normalmap; }
		static inline UniformValueFloat *time(){ return &_time; }
		static inline UniformValueVector2 *pixelSize(){ return &_pixelSize; }
		static inline UniformValueVector2 *pixelOffset() { return &_pixelOffset; }
		static inline UniformValueVector3 *highlightColor() { return &_highlightColor; }
		static inline UniformValueFloat* fogStart() { return &_fogStart; }
		static inline UniformValueFloat* fogDensity() { return &_fogDensity; }
		static inline UniformValueFloat* fogScale() { return &_fogScale; }
		static inline UniformValueVector3* fogColor() { return &_fogColor; }
		static inline UniformValueTexture *paintmask() { return &_paintmask; }

		static void initSamplers();
		static void resetSamplers();
	private:
		static void initSampler(UniformValueTexture *sampler, int index);
		static UniformValueVector3 _cameraPosition;
		static UniformValueMatrix _projection;
		static UniformValueMatrix3 _colorTransform;
		static UniformValueTexture _texture;
		static UniformValueTexture _lightmap;
		static UniformValueTexture _environment;
		static UniformValueTexture _depthmap;
		static UniformValueTexture _normalmap;
		static UniformValueFloat _time;
		static UniformValueVector2 _pixelSize;
		static UniformValueVector2 _pixelOffset;
		static UniformValueVector3 _highlightColor;
		static UniformValueFloat _fogStart;
		static UniformValueFloat _fogDensity;
		static UniformValueFloat _fogScale;
		static UniformValueVector3 _fogColor;
		static UniformValueTexture _paintmask;

		static UniformValueTexture* _samplers[GraphicsConstants::Samplers2DLimit];
	};
}

#endif /*UNIFORM_INFO_H*/