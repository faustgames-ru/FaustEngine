#include "Errors.h"
#include "Effect.h"
#include "Uniform.h"
#include "UniformInfo.h"
#include "VertexFormat.h"

namespace graphics
{
	Effect::Effect() : _attributesMax(0), _attributesMask(0), _samperIndex(-1)
	{
	}

	Effect::~Effect()
	{
	}

	Effect *Effect::addUniform(UniformInfo *uniformInfo, UniformValue *uniformValue)
	{
		_uniforms.addEmpty()->init(uniformInfo, uniformValue);
		return this;
	}
	
	Effect *Effect::addAttribute(AttributeInfo *attribute)
	{
		_attributes.add(attribute);
		return this;
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
				//exit(1);
			}
		}
		else
		{
		}
	}

	GLuint Effect::getHandle()
	{
		return _shaderProgram;
	}

	unsigned int Effect::getAttributesMask()
	{
		return _attributesMask;
	}

	unsigned int Effect::getAttributesMax()
	{
		return _attributesMax;
	}
	
	int Effect::generateSamplerIndex()
	{
		return ++_samperIndex;
	}


	void Effect::create(const char *vertexShaderCode, const char *pixelShaderCode)
	{
		_vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderCode);
		_pixelShader = createShader(GL_FRAGMENT_SHADER, pixelShaderCode);

		_shaderProgram = glCreateProgram();
		Errors::check(Errors::CreateProgram);
		
		glAttachShader(_shaderProgram, _vertexShader);
		Errors::check(Errors::AttachShader);
		
		glAttachShader(_shaderProgram, _pixelShader);
		Errors::check(Errors::AttachShader);
		
		for (int i = 0; i < _attributes.count; i++)
		{
			int location = (int)_attributes.data[i]->getLocation();
			if (_attributesMax < location)
				_attributesMax = location;
			_attributesMask |= _attributes.data[i]->getMask();
			glBindAttribLocation(_shaderProgram, location, _attributes.data[i]->getName());
			Errors::check(Errors::BindAttribLocation);
		}
		_attributesMax++;

		glLinkProgram(_shaderProgram);
		Errors::check(Errors::LinkProgram);
		
		//CheckLink(_shaderProgram);

		glUseProgram(_shaderProgram);
		Errors::check(Errors::UseProgram);

		for (int i = 0; i < _uniforms.count; i++)
		{
			_uniforms.data[i].create(this);
		}
	}

	void Effect::applyShader()
	{
		glUseProgram(_shaderProgram);
		Errors::check(Errors::UseProgram);
	}

	void Effect::applyUniforms()
	{
		for (int i = 0; i < _uniforms.count; i++)
		{
			_uniforms.data[i].getValueContainer()->apply(&(_uniforms.data[i]));
		}
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

	void Effect::applyState(unsigned int prevMask, unsigned int newMask, int count)
	{
		unsigned int currentBit(0x1);
		unsigned int isEnabledBit;
		unsigned int needEnableBit;
		for (int i = 0; i < count; ++i)
		{
			isEnabledBit = (prevMask & currentBit);
			needEnableBit = (newMask & currentBit);
			if (isEnabledBit != needEnableBit)
			{
				if (isEnabledBit == currentBit)
				{
					glDisableVertexAttribArray(i);
					Errors::check(Errors::EnableVertexAttribArray);
				}
				else
				{
					glEnableVertexAttribArray(i);
					Errors::check(Errors::EnableVertexAttribArray);
				}
			}
			currentBit = currentBit << 1;
		}
	}

	void Effect::applyVertexData(VertexFormat * vertexFormat, void* vertexData)
	{
		for (int i = 0; i < _attributes.count; ++i)
		{				
			int location = _attributes.data[i]->getLocation();
			AttributeFormat *format = vertexFormat->getFormat(location);
			glVertexAttribPointer(location, format->ElementsCount, format->ElementType, format->Normalized, vertexFormat->getStride(), (char *)vertexData + format->Offset);
			Errors::check(Errors::EnableVertexAttribArray);
		}
	}
}