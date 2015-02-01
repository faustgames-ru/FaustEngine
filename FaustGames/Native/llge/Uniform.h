#ifndef UNIFORM_H
#define UNIFORM_H

#include "graphics.h"
#include "Matrix.h"
#include "Vector.h"

namespace graphics
{
	class Uniform
	{
	private:
		GLuint _parameterHandler;
		UniformInfo *_info;
		int _samplerIndex;
	public:
		UniformValue *valueContainer;
		Uniform();
		~Uniform();
		inline int getSamplerIndex(){ return _samplerIndex; }
		UniformInfo *getInfo();
		void init(UniformInfo *info, UniformValue *value);
		void create(Effect *effect);
		GLuint getHandle();
	};

	class UniformValue
	{
	protected:
		Uniform *_uniform;
	public:
		void setUniform(Uniform *uniform)
		{
			_uniform = uniform;
		}
		virtual void apply() = 0;
	};
	
	class UniformValueFloat : public UniformValue
	{
	private:
		float _value;
		bool _equal;
	public:
		UniformValueFloat();
		void setValue(float value);
		virtual void apply();
	};
	
	class UniformValueMatrix : public UniformValue
	{
	private:
		core::MatrixContainer _value;
		bool _equal;
	public:
		UniformValueMatrix();
		void setValue(const core::MatrixContainer &value);
		virtual void apply();
	};

	class UniformValueTexture : public UniformValue
	{
	private:
		GLuint _value;
		bool _equal;
	public:
		UniformValueTexture();
		void setValue(GLuint value);
		virtual void apply();
	};
}

#endif /*UNIFORM_H*/