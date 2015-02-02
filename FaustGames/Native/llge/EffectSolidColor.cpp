#include "EffectSolidColor.h"
#include "Uniforms.h"
#include "Attributes.h"

namespace graphics
{
	EffectSolidColor::EffectSolidColor()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::color());
	}

	EffectSolidColor::~EffectSolidColor()
	{
	}

	void EffectSolidColor::create()
	{
		_effect.create(_vertexShader.c_str(), _pixelShader.c_str());
	}
	
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
}