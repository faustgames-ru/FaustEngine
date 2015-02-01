#ifndef EFFECT_H
#define EFFECT_H

#include "graphics.h"
#include "Uniform.h"
#include "AttributeInfo.h"
#include "StaticArray.h"
#include "VertexFormat.h"

namespace graphics
{
	class Effect
	{
	private:
		std::string _vertexShaderCode;
		std::string _pixelShaderCode;
		core::StaticArray<Uniform, 32> _uniforms;
		core::StaticArray<AttributeInfo *, 32> _attributes;
		unsigned int _attributesMask;
		int _attributesMax;
		int _samperIndex;
		GLuint _shaderProgram;
		GLuint _vertexShader;
		GLuint _pixelShader;
		GLuint createShader(GLenum type, const char *code);
	public:
		Effect::Effect();
		Effect::~Effect();
		void setCode(const char *vertexShaderCode, const char *pixelShaderCode);
		Effect *addUniform(UniformInfo *uniformInfo, UniformValue *uniformValue);
		Effect *addAttribute(AttributeInfo *attribute);
		GLuint getHandle();
		unsigned int getAttributesMask();
		unsigned int getAttributesMax();
		int generateSamplerIndex();
		void create();
		void applyShader();
		void applyUniforms();
		void cleanup();
		void applyVertexData(VertexFormat * vertexFormat, void* vertexData);
		static void applyState(unsigned int prevMask, unsigned int newMask, int count);

	};
}

#endif /*EFFECT_H*/