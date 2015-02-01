#ifndef ERRORS_H
#define ERRORS_H

#include "graphics.h"

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
			CreateTexture = 19,
			BindTexture = 20,
			TexImage2D = 21,
			TexParameteri = 22,
			DeleteTexture = 23,
			ActiveTexture = 24,
			EnumSize = 25,
		};

		static std::string _names[EnumSize];

		static const char * getActionName(Errors::e actionType);
		static void check(Errors::e actionType);
	};
}

#endif /*ERRORS_H*/