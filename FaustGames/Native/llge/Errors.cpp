#include "Errors.h"

namespace graphics
{
	const char * Errors::getActionName(Errors::e actionType)
	{
		return _names[actionType].c_str();
	}
	
	void Errors::check(Errors::e actionType)
	{
#ifdef _DEBUG
		GLenum error(GL_NO_ERROR);
		while ((error = glGetError()) != GL_NO_ERROR) 
		{
			throw std::exception(getActionName(actionType));
		}
#endif
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
	};
}