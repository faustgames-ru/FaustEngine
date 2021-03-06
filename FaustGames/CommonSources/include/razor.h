#ifndef RAZOR_H
#define RAZOR_H

#ifdef __ANDROID__
#include <jni.h>
#define API_CALL JNICALL
#define DLLEXPORT JNIEXPORT
#define OVERRIDE
#define nullptr 0
#else 
#ifdef __APPLE__
#define API_CALL
#define DLLEXPORT
#define OVERRIDE
#define nullptr 0
#else
#define API_CALL __stdcall
#define DLLEXPORT __declspec( dllexport )
#define OVERRIDE override
#endif
#endif

#include "BaseObjects.h"

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong;
typedef const char * String;
typedef void * IntPtr;

namespace razor
{

	class IRazorGame : IBaseObject
	{
	public:
		virtual void API_CALL setRootPath(String rootPath) = 0;
		virtual void API_CALL updateMouse(int rawX, int rawY, int windowX, int windowY, uint buttons) = 0;
		virtual void API_CALL updateEnvironment(int w, int h, float ellapsedTime) = 0;
		virtual void API_CALL load() = 0;
		virtual void API_CALL update() = 0;
		virtual void API_CALL render() = 0;
		virtual void API_CALL release() = 0;
	};

	extern "C" DLLEXPORT void API_CALL initRazorRenderContext();
	extern "C" DLLEXPORT IRazorGame* API_CALL createRazorGame();
}

#endif /*RAZOR_H*/