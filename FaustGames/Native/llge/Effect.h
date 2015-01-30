#ifndef EFFECT_H
#define EFFECT_H

#include "graphics.h"
#include "Uniform.h"
#include "StaticArray.h"

namespace graphics
{
	class Effect
	{
	private:
		std::string _vertexShaderCode;
		std::string _pixelShaderCode;
		core::StaticArray<Uniform, 32> _uniforms;
		GLuint _shaderProgram;
		GLuint _vertexShader;
		GLuint _pixelShader;
		GLuint createShader(GLenum type, const char *code);
	public:
		Effect::Effect();
		Effect::~Effect();
		void setCode(const char *vertexShaderCode, const char *pixelShaderCode);
		Effect *addUniform(UniformInfo *uniformInfo, UniformValue *uniformValue);
		GLuint getHandle();
		void create();
		void applyShader();
		void applyUniforms();
		void cleanup();
	};
}

#endif /*EFFECT_H*/