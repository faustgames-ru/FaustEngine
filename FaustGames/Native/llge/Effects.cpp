#include "Effects.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "ObjectsPool.h"

namespace graphics
{
	Effects::Effects()
	{
	}

	void Effects::create()
	{
		_solid.getEffect()->create();
		_solidColor.getEffect()->create();
		_textureColor.getEffect()->create();
	}

	void Effects::cleanup()
	{
		_solid.getEffect()->cleanup();
		_solidColor.getEffect()->cleanup();
		_textureColor.getEffect()->cleanup();
	}
	
	EffectSolid * Effects::solid()
	{
		return &_solid;
	}

	EffectSolidColor * Effects::solidColor()
	{
		return &_solidColor;
	}

	EffectTextureColor * Effects::textureColor()
	{
		return &_textureColor;
	}

	Effects::~Effects()
	{
	}

	Effects * Effects::instanciate()
	{
		return new Effects();
	}
	
	void Effects::deinstanciate(Effects *value)
	{
		delete value;
	}

	core::Lazy<Effects> Effects::_instance(Effects::instanciate, Effects::deinstanciate);


	Effects * Effects::instance()
	{
		return _instance.value();
	}

	EffectSolid::EffectSolid()
	{
		_effect.setCode(_vertexShader.c_str(), _pixelShader.c_str());
		_effect.addUniform(Uniforms::projection(), &_projection);
		_effect.addAttribute(Attributes::position());
	}
	
	EffectSolid::~EffectSolid()
	{
	}

	Effect *EffectSolid::getEffect()
	{
		return &_effect;
	}
	UniformValueMatrix *EffectSolid::getProjection()
	{
		return &_projection;
	}

	EffectSolidColor::EffectSolidColor()
	{
		_effect.setCode(_vertexShader.c_str(), _pixelShader.c_str());
		_effect.addUniform(Uniforms::projection(), &_projection);
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::color());
	}

	EffectSolidColor::~EffectSolidColor()
	{
	}
	
	Effect *EffectSolidColor::getEffect()
	{
		return &_effect;
	}
	
	UniformValueMatrix *EffectSolidColor::getProjection()
	{
		return &_projection;
	}

	EffectTextureColor::EffectTextureColor()
	{
		_effect.setCode(_vertexShader.c_str(), _pixelShader.c_str());
		_effect.addUniform(Uniforms::projection(), &_projection);
		_effect.addUniform(Uniforms::texture(), &_texture);
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}
	EffectTextureColor::~EffectTextureColor()
	{
	}
	Effect *EffectTextureColor::getEffect()
	{
		return &_effect;
	}
	
	UniformValueMatrix *EffectTextureColor::getProjection()
	{
		return  &_projection;
	}

	UniformValueTexture *EffectTextureColor::getTexture()
	{
		return  &_texture;
	}
	


	std::string EffectSolid::_pixelShader(
"\
void main() \
{\
gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\
}\
");

	std::string EffectSolid::_vertexShader(
"\
attribute vec3 position;\
uniform mat4 projection;\
\
void main()\
{\
gl_Position = projection *  vec4(position, 1.0);\
}\
");

	std::string EffectSolidColor::_pixelShader(
"\
varying vec4 _color;\
\
void main() \
{\
gl_FragColor = _color;\
}\
");

	std::string EffectSolidColor::_vertexShader(
"\
uniform mat4 projection;\
attribute vec3 position;\
attribute vec4 color;\
\
varying vec4 _color;\
\
void main()\
{\
gl_Position = projection *  vec4(position, 1.0);\
_color = color;\
}\
");


	std::string EffectTextureColor::_pixelShader(
"\
uniform sampler2D texture;\
\
varying vec4 _color;\
varying vec2 _textureCoords;\
\
void main()\
{\
vec4 textureColor = texture2D(texture, _textureCoords);\
gl_FragColor = textureColor*_color;\
}\
");

	std::string EffectTextureColor::_vertexShader(
"\
uniform mat4 projection;\
attribute vec3 position;\
attribute vec2 textureCoords;\
attribute vec4 color;\
\
varying vec4 _color;\
varying vec2 _textureCoords;\
\
void main()\
{\
gl_Position = projection *  vec4(position, 1.0);\
_color = color;\
_textureCoords = textureCoords;\
}\
");

}