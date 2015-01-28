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
			EnumSize= 11,
		};

		static std::string _names[EnumSize];

		static const char * getActionName(Errors::e actionType);
		static void check(Errors::e actionType);
	};
}

#endif /*ERRORS_H*/