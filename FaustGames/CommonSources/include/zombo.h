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

	class IZomboEditorMouse : IBaseObject
	{
	public:
		virtual void API_CALL update(int mouseX, int mouseY, uint mouseButtons) = 0;
	};
	
	class IZomboEditorInput : IBaseObject
	{
	public:
		virtual IZomboEditorMouse* API_CALL getEditorMouse() = 0;
	};

	class IZomboEditorViewport : IBaseObject
	{
	public:
		virtual void API_CALL update(int width, int height) = 0;
	};

	class IZomboEditorCamera: IBaseObject
	{
	public:
		virtual IntPtr API_CALL getMode() = 0;
		virtual void API_CALL setMode(String modeName) = 0;
		virtual void API_CALL setScale(float scale) = 0;
		virtual void API_CALL setFov(float scale) = 0;
		virtual float API_CALL getScale() = 0;
		virtual float API_CALL getFov() = 0;
	};

	class IZomboEditor : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getMode() = 0;
		virtual void API_CALL setMode(String modeName) = 0;
		virtual bool API_CALL isUndoAvaliable() = 0;
		virtual bool API_CALL isRedoAvaliable() = 0;
		virtual void API_CALL undo() = 0;
		virtual void API_CALL redo() = 0;
		virtual void API_CALL init() = 0;
		virtual void API_CALL update(float ellapsedTime) = 0;
		virtual void API_CALL render() = 0;
		virtual void API_CALL release() = 0;
	};

	extern "C" DLLEXPORT IZomboGame* API_CALL createZomboGame();
	
	extern "C" DLLEXPORT IZomboEditor* API_CALL getZomboEditor();
	extern "C" DLLEXPORT IZomboEditorInput* API_CALL getZomboEditorInput();
	extern "C" DLLEXPORT IZomboEditorViewport* API_CALL getZomboEditorViewport();
	extern "C" DLLEXPORT IZomboEditorCamera* API_CALL getZomboEditorCamera();
	extern "C" DLLEXPORT void API_CALL initZomboRenderContext();
}

#endif /*ZOMBO_H*/