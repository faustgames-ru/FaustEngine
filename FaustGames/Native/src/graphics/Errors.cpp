#include "Errors.h"

namespace graphics
{
	const char * Errors::getActionName(Errors::e actionType)
	{
		return _names[actionType].c_str();
	}
	
	void Errors::check(Errors::e actionType)
	{

//#ifdef _DEBUG
		GLenum error(GL_NO_ERROR);
		while ((error = glGetError()) != GL_NO_ERROR) 
		{
#ifdef __ANDROID__
			__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", getActionName(actionType));
#endif
			//throwException(getActionName(actionType));
			
			//throw std::exception();
		}
//#endif
	}

	std::string Errors::_names[Errors::EnumSize] = 
	{
		"UnknownAction",
		"CreateShader",
		"ShaderSource",
		"CompileShader",
		"CreateProgram",
		"AttachShader",
		"LinkProgram",
		"UseProgram",
		"DetachShader",
		"DeleteProgram",
		"DeleteShader",
		"GetUniformLocation",
		"GetAttribLocation",
		"BindAttribLocation",
		"Uniform1f",
		"UniformMatrix4fv",
		"EnableVertexAttribArray",
		"DisableVertexAttribArray",
		"DrawElements",
		"GenTextures",
		"BindTexture",
		"TexImage2D",
		"TexParameteri",
		"DeleteTexture",		
		"ActiveTexture",
		"GenFramebuffers",
		"GenRenderbuffers",
		"BindFramebuffer",
		"FramebufferTexture2D",
		"BindRenderbuffer",
		"RenderbufferStorage",
		"FramebufferRenderbuffer",
		"DeleteFramebuffers",
		"DeleteRenderbuffers",
		"CheckFramebufferStatus", 
		"Viewport",
		"Clear",
		"Finish",
		"VertexAttribPointer",
		"GenBuffers",
		"DeleteBuffers",
		"BindBuffer",
		"BufferData",
		"Uniform3f",
	};
}