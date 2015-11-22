#ifndef ZOMBO_H
#define ZOMBO_H

#ifdef __ANDROID__
#include <jni.h>
#define API_CALL JNICALL
#define DLLEXPORT JNIEXPORT
#define OVERRIDE
#else 
#ifdef __APPLE__
#define API_CALL
#define DLLEXPORT
#define OVERRIDE
#else
#define API_CALL __stdcall
#define DLLEXPORT __declspec( dllexport )
#define OVERRIDE override
#endif
#endif

#include "BaseObjects.h"

typedef unsigned short ushort;
typedef unsigned int uint;
typedef const char * String;
typedef void * IntPtr;

namespace zombo
{
	class IZomboGame : IBaseObject
	{
	public:
		virtual void API_CALL load() = 0;
		virtual void API_CALL update(float ellapsedTime) = 0;
		virtual void API_CALL render() = 0;
	};

	extern "C" DLLEXPORT IZomboGame * API_CALL createZomboGame();
}

#endif /*ZOMBO_H*/