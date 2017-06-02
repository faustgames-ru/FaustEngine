#ifdef __ANDROID__
#include <jni.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif TARGET_OS_MAC
#include <OpenGL/gl.h>
#endif
#else
#include "..\..\llge\stdafx.h"
#include <windows.h>
#include "glew.h"
#endif
#include <string>
#include <vector>
