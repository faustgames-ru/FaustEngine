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
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_updateEnvironment (IRazorGame * classInstance, int w, int h, float ellapsedTime)
	{
		classInstance->updateEnvironment(w, h, ellapsedTime);
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_load (IRazorGame * classInstance)
	{
		classInstance->load();
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_update (IRazorGame * classInstance)
	{
		classInstance->update();
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_render (IRazorGame * classInstance)
	{
		classInstance->render();
	}
	
	extern "C" DLLEXPORT void API_CALL razor_RazorGame_release (IRazorGame * classInstance)
	{
		classInstance->release();
	}
	
}

