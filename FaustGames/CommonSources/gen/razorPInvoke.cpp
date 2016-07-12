/* ============================================================== */
/* This file is automatically generated. Please do not modify it. */
/* ============================================================== */

#include "razor.h"

namespace razor
{
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_setRootPath (IRazorGame * classInstance, String rootPath)
	{
		classInstance->setRootPath(rootPath);
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_load (IRazorGame * classInstance, int w, int h)
	{
		classInstance->load(w, h);
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_update (IRazorGame * classInstance, int w, int h, float ellapsedTime)
	{
		classInstance->update(w, h, ellapsedTime);
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_render (IRazorGame * classInstance, int w, int h, float ellapsedTime)
	{
		classInstance->render(w, h, ellapsedTime);
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_release (IRazorGame * classInstance)
	{
		classInstance->release();
	}
	
}

