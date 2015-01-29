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
		std::vector<Uniform *> _uniforms;
		GLuint _shaderProgram;
		GLuint _vertexShader;
		GLuint _pixelShader;
		GLuint createShader(GLenum type, const char *code);
	public:
		Effect::Effect(const char *vertexShaderCode, const char *pixelShaderCode);
		void addUniform(UniformInfo *uniformInfo);
		void create();
		void cleanup();
	};
}

#endif /*EFFECT_H*/