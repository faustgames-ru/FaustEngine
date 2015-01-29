#include "Errors.h"
#include "Effect.h"
#include "Uniform.h"
#include "UniformInfo.h"

namespace graphics
{
	Effect::Effect(const char *vertexShaderCode, const char *pixelShaderCode)
	{
		_vertexShaderCode = vertexShaderCode;
		_pixelShaderCode = pixelShaderCode;
	}

	void Effect::addUniform(UniformInfo *uniformInfo)
	{
		Uniform *uniform = new Uniform(uniformInfo);
		_uniforms.push_back(uniform);
	}


	GLuint Effect::createShader(GLenum type, const GLchar *code)
	{
		GLuint shader = glCreateShader(type);
		Errors::check(Errors::CreateShader);
		
		glShaderSource(shader, 1, &code, 0);
		Errors::check(Errors::ShaderSource);
		
		glCompileShader(shader);
		Errors::check(Errors::CompileShader);
		
		return shader;
	}


	void CheckLink(GLuint id)
	{
		//CheckAndLog(id,GL_LINK_STATUS,"conexion");
		GLint compiled = 0;
		GLint length = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &compiled);
		if (compiled == 0)
		{
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			if (length > 1) {
				std::string log(length, '\0');
				glGetProgramInfoLog(id, length, &length, &log[0]);
				exit(1);
			}
		}
		else
		{
		}
	}

	void Effect::create()
	{
		_vertexShader = createShader(GL_VERTEX_SHADER, _vertexShaderCode.c_str());
		_pixelShader = createShader(GL_FRAGMENT_SHADER, _pixelShaderCode.c_str());

		_shaderProgram = glCreateProgram();
		Errors::check(Errors::CreateProgram);
		
		glAttachShader(_shaderProgram, _vertexShader);
		Errors::check(Errors::AttachShader);
		
		glAttachShader(_shaderProgram, _pixelShader);
		Errors::check(Errors::AttachShader);
		
		glLinkProgram(_shaderProgram);
		Errors::check(Errors::LinkProgram);
		
		glUseProgram(_shaderProgram);
		Errors::check(Errors::UseProgram);
	}

	void Effect::cleanup()
	{
		glDetachShader(_shaderProgram, _pixelShader);
		Errors::check(Errors::DetachShader);
		glDetachShader(_shaderProgram, _vertexShader);
		Errors::check(Errors::DetachShader);
		glDeleteProgram(_shaderProgram);
		Errors::check(Errors::DeleteProgram);
		glDeleteShader(_vertexShader);
		Errors::check(Errors::DeleteShader);
		glDeleteShader(_pixelShader);
		Errors::check(Errors::DeleteShader);
		_shaderProgram = 0;
		_pixelShader = 0;
		_vertexShader = 0;
		
	}
}