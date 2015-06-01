#ifdef __ANDROID__
#include <jni.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#else
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include "..\..\llge\stdafx.h"
#include <windows.h>
#include "glew.h"
#endif
#endif

#include <string>
#include <vector>
