#ifndef CONFIG_DEFINES_H
#define CONFIG_DEFINES_H

#ifdef EFFECTS_BASIC
#define INCLUDE_EFFECTS "EffectsBasic.h"
#define INCLUDE_RENDER_CONVERTER "RenderConverterBasic.h"
#define INCLUDE_VERTEX_FORMATS "VertexFormatsBasic.h"
#define EFFECTS_CALL_CREATE graphics::EffectsBasic::create();
#define EFFECTS_CALL_CLEANUP graphics::EffectsBasic::cleanup();
#define FORMATS_CALL_CREATE graphics::VertexFormatsBasic::create();
#define VERTEX_FORMATS graphics::VertexFormatsBasic
#define RENDER_CONVERTER_CLASS graphics::RenderConverterBasic
#else
#define INCLUDE_EFFECTS "Effects.h"
#define INCLUDE_RENDER_CONVERTER "RenderConverter.h"
#define INCLUDE_VERTEX_FORMATS "VertexFormats.h"
#define EFFECTS_CALL_CREATE graphics::Effects::create();
#define EFFECTS_CALL_CLEANUP graphics::Effects::cleanup();
#define FORMATS_CALL_CREATE graphics::VertexFormats::create();
#define VERTEX_FORMATS graphics::VertexFormats
#define RENDER_CONVERTER_CLASS graphics::RenderConverter
#endif

#endif /*CONFIG_DEFINES_H*/