#ifndef ZOMBO_H
#define ZOMBO_H

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
typedef unsigned long ulong;
typedef const char * String;
typedef void * IntPtr;

namespace zombo
{
	class IZomboGame : IBaseObject
	{
	public:
		virtual void API_CALL setRootPath(String rootPath) = 0;
		virtual void API_CALL load(int w, int h) = 0;
		virtual void API_CALL update(int w, int h, float ellapsedTime) = 0;
		virtual void API_CALL render(int w, int h, float ellapsedTime) = 0;
		virtual void API_CALL release() = 0;
	};

	extern "C" DLLEXPORT IZomboGame * API_CALL createZomboGame();
	extern "C" DLLEXPORT void API_CALL initZomboRenderContext();
}

#endif /*ZOMBO_H*/