#include "EffectTextureColor.h"
#include "Uniforms.h"
#include "Attributes.h"

namespace graphics
{
	EffectTextureColor::EffectTextureColor()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}
	EffectTextureColor::~EffectTextureColor()
	{
	}

	void EffectTextureColor::create()
	{
		_effect.create(_vertexShader.c_str(), _pixelShader.c_str());
	}

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