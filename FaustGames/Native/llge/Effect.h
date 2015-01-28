#ifndef EFFECT_H
#define EFFECT_H

#include "graphics.h"

namespace graphics
{
	class Effect
	{
	private:
		std::string _vertexShaderCode;
		std::string _pixelShaderCode;
		GLuint _shaderProgram;
		GLuint _vertexShader;
		GLuint _pixelShader;
		GLuint createShader(GLenum type, const char *code);
	public:
		Effect(const char *vertexShaderCode, const char *pixelShaderCode, const UniformInfo * uniformInfos[]);
		void create();
		void release();
	};
}

#endif /*EFFECT_H*/