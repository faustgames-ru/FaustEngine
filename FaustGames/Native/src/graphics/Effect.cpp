#include "Errors.h"
#include "Effect.h"
#include "Uniform.h"
#include "UniformInfo.h"
#include "VertexFormat.h"

namespace graphics
{
	int Effect::_shadersCounter(0);

	Effect::Effect() : _attributesMax(0), _attributesMask(0), _samperIndex(-1), _shaderId(_shadersCounter++)
	{
		_shaderMask = 0x1 << _shaderId;
	}

	Effect::~Effect()
	{
	}

	Effect *Effect::addConstant(const char *name, UniformType::e type)
	{
		_constants.addEmpty()->init(name, type);
		return this;
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


	void CheckShader(GLuint id)
	{
		GLint compiled = 0;
		GLint length = 0;
		GLint deleteStatus = 0;
		GLint shaderSourceLength = 0;
		GLint shaderType = 0;
		glGetShaderiv(id, GL_SHADER_TYPE, &shaderType);
		glGetShaderiv(id, GL_DELETE_STATUS, &deleteStatus);
		glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &shaderSourceLength);


		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		Errors::check(Errors::UnknownAction);
		if (compiled == 0)
		{
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			Errors::check(Errors::UnknownAction);
			if (length > 1) {
				std::string log(length, '\0');
				glGetShaderInfoLog(id, length, &length, &log[0]);
				Errors::check(Errors::UnknownAction);
				//exit(1);
			}
		}
	}

	GLuint Effect::createShader(GLenum type, const char *code, int size)
	{
		GLuint shader = glCreateShader(type);
		Errors::check(Errors::CreateShader);
		
		glShaderSource(shader, 1, &code, &size);
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


	void Effect::create(const char *vertexShaderCode, int vertexShaderSize, const char *pixelShaderCode, int pixelShaderSize)
	{
		_pixelShader = createShader(GL_FRAGMENT_SHADER, pixelShaderCode, pixelShaderSize);
		_vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderCode, vertexShaderSize);

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
		
		glUseProgram(_shaderProgram);
		Errors::check(Errors::UseProgram);

		for (int i = 0; i < _uniforms.count; i++)
		{
			_uniforms.data[i].create(this);
		}
		for (int i = 0; i < _constants.count; i++)
		{
			_constants.data[i].create(this);
		}

		_constantsChanged = true;
	}

	void Effect::applyShader()
	{
		glUseProgram(_shaderProgram);
		Errors::check(Errors::UseProgram);
	}

	void Effect::applyUniforms()
	{
		if (_constantsChanged)
		{
			for (int i = 0; i < _constants.count; i++)
			{
				_constants.data[i].apply();
			}
			_constantsChanged = false;
		}
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
			Errors::check(Errors::VertexAttribPointer);
		}
	}

	EffectConstant * Effect::findConstant(const char *name)
	{
		for (int i = 0; i < _constants.count; i++)
		{
			if (_constants.data[i].nameEqualsTo(name))
				return &(_constants.data[i]);
		}
		return 0;
	}
}