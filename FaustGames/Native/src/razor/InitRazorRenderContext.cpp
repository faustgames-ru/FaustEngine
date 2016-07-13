#include "razor.h"
#include <glew.h>

namespace razor
{
	extern "C" DLLEXPORT void API_CALL initRazorRenderContext()
	{
#ifdef __ANDROID__
#else
#ifdef __APPLE__
#else
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			switch (err)
			{
			case GLEW_ERROR_NO_GL_VERSION:
				break;  /* missing GL version */
			case GLEW_ERROR_GL_VERSION_10_ONLY:
				break;  /* Need at least OpenGL 1.1 */
			case GLEW_ERROR_GLX_VERSION_11_ONLY:
				break;  /* Need at least GLX 1.2 */
			default:
				break;
			}
			//throw std::exception("fail to init glew");
		}
#endif
#endif
	}
}
