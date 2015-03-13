#ifndef EFFECT_H
#define EFFECT_H

#include "graphics_classes.h"
#include "Uniform.h"
#include "AttributeInfo.h"
#include "VertexFormat.h"
#include "EffectConstant.h"

namespace graphics
{
	class Effect
	{
	public:
		Effect();
		~Effect();
		Effect *addConstant(const char *name, UniformType::e type);
		Effect *addUniform(UniformInfo *uniformInfo, UniformValue *uniformValue);
		Effect *addAttribute(AttributeInfo *attribute);
		GLuint getHandle();
		unsigned int getAttributesMask();
		unsigned int getAttributesMax();
		int generateSamplerIndex();
		void create(const char *vertexShaderCode, int vertexShaderSize, const char *pixelShaderCode, int pixelShaderSize);
		void applyShader();
		void applyUniforms();
		void cleanup();
		void applyVertexData(VertexFormat * vertexFormat, void* vertexData);
		EffectConstant * findConstant(const char *name);
		static void applyState(unsigned int prevMask, unsigned int newMask, int count);
		inline int getShaderId(){ return _shaderId; }
		inline unsigned int getShaderMask(){ return _shaderMask; }
		inline void setConstantsChanged() { _constantsChanged = true; }
	protected:
	private:
		core::StaticArray<EffectConstant, 32> _constants;
		core::StaticArray<Uniform, 32> _uniforms;
		core::StaticArray<AttributeInfo *, 32> _attributes;
		unsigned int _attributesMask;
		int _attributesMax;
		int _samperIndex;
		GLuint _shaderProgram;
		GLuint _vertexShader;
		GLuint _pixelShader;
		GLuint createShader(GLenum type, const char *code, int size);
		int _shaderId;
		int _shaderMask;
		bool _constantsChanged;
		static int _shadersCounter;
	};
}

#endif /*EFFECT_H*/