#ifndef CONFIG_DEFINES_H
#define CONFIG_DEFINES_H

#ifdef EFFECTS_BASIC
#define INCLUDE_EFFECTS "EffectsBasic.h"
#define INCLUDE_RENDER_CONVERTER "RenderConverterBasic.h"
#define INCLUDE_VERTEX_FORMATS "VertexFormatsBasic.h"
#define EFFECTS_CALL_CREATE EffectsBasic::create();
#define EFFECTS_CALL_CLEANUP EffectsBasic::cleanup();
#define FORMATS_CALL_CREATE VertexFormatsBasic::create();
#define RENDER_CONVERTER_CLASS graphics::RenderConverterBasic
#else
#define INCLUDE_EFFECTS "Effects.h"
#define INCLUDE_RENDER_CONVERTER "RenderConverter.h"
#define INCLUDE_VERTEX_FORMATS "VertexFormats.h"
#define EFFECTS_CALL_CREATE Effects::create();
#define EFFECTS_CALL_CLEANUP Effects::cleanup();
#define FORMATS_CALL_CREATE VertexFormats::create();
#define RENDER_CONVERTER_CLASS graphics::RenderConverter
#endif

#endif /*CONFIG_DEFINES_H*/