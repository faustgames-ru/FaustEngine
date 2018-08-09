#include "Errors.h"

//#define OGL_ERRORS_CHECK

namespace graphics
{
	const char * Errors::getActionName(Errors::e actionType)
	{
		return _names[actionType].c_str();
	}
	
	template <typename T>
	std::string to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}

	void Errors::check(const char* errorText)
	{
#ifdef OGL_ERRORS_CHECK
		GLenum error(GL_NO_ERROR);
		while ((error = glGetError()) != GL_NO_ERROR)
		{
#ifdef __ANDROID__
			__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", errorText);
#endif
#ifdef __APPLE__
            fprintf(stderr, errorText);
            fprintf(stderr, "\n");
#endif
			auto errorText1 = errorText;
			//throwException(getActionName(actionType));

			//throw std::exception();
		}
#endif
	}
	/*
	void Errors::check(Errors::e actionType)
	{
		
#ifdef OGL_ERRORS_CHECK

		GLenum error(GL_NO_ERROR);
		if ((error = glGetError()) != GL_NO_ERROR) 
		{
#ifdef __ANDROID__
			__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", getActionName(actionType));
			__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", to_string(error).c_str());
#else

#ifdef __APPLE__
            fprintf(stderr, getActionName(actionType));
            fprintf(stderr, "\n");

            fprintf(stderr, to_string(error).c_str());
            fprintf(stderr, "\n");

#else
#ifdef _DEBUG
			throw std::exception();

#endif
#endif
#endif
			//throwException(getActionName(actionType));
			
			//throw std::exception();
		}
#endif
		
	}
*/
    void Errors::check(Errors::e actionType)
    {
        
        
        GLenum error(GL_NO_ERROR);
        if ((error = glGetError()) != GL_NO_ERROR)
        {

            fprintf(stderr, "%s", getActionName(actionType));
            fprintf(stderr, "\n");
            
        }
        
    }
    
	void Errors::rise(Errors::e actionType)
	{

		//#ifdef _DEBUG
#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", getActionName(actionType));
#endif
		//throwException(getActionName(actionType));

		//throw std::exception();
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
		"Uniform2f",
		"CompressedTexImage2D",
		"UniformMatrix3fv"
	};
}
