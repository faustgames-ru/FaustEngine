#ifndef ERRORS_H
#define ERRORS_H

#include "graphics_classes.h"

namespace graphics
{
	class Errors
	{
	public:
		enum e
		{
			UnknownAction = 0,
			CreateShader = 1,
			ShaderSource = 2,
			CompileShader = 3,
			CreateProgram = 4,
			AttachShader = 5,
			LinkProgram = 6,
			UseProgram = 7,
			DetachShader=8,
			DeleteProgram=9,
			DeleteShader=10,
			GetUniformLocation = 11,
			GetAttribLocation = 12,
			BindAttribLocation = 13,
			Uniform1f = 14, 
			UniformMatrix4fv = 15,
			EnableVertexAttribArray=16,
			DisableVertexAttribArray = 17,
			DrawElements = 18,
			GenTextures = 19,
			BindTexture = 20,
			TexImage2D = 21,
			TexParameteri = 22,
			DeleteTexture = 23,
			ActiveTexture = 24,
			GenFramebuffers = 25,
			GenRenderbuffers = 26,
			BindFramebuffer = 27,
			FramebufferTexture2D = 28,
			BindRenderbuffer = 29,
			RenderbufferStorage = 30,
			FramebufferRenderbuffer = 31,
			DeleteFramebuffers = 32,
			DeleteRenderbuffers = 33,
			CheckFramebufferStatus = 34,
			Viewport = 35,
			Clear = 36,
			Finish = 37,
			EnumSize = 38,
		};

		static std::string _names[EnumSize];

		static const char * getActionName(Errors::e actionType);
		static void check(Errors::e actionType);
	protected:
	private:
	};
}

#endif /*ERRORS_H*/