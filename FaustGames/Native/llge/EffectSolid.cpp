#include "EffectSolid.h"
#include "Uniforms.h"
#include "Attributes.h"

namespace graphics
{
	EffectSolid::EffectSolid()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addAttribute(Attributes::position());
	}

	EffectSolid::~EffectSolid()
	{
	}

	void EffectSolid::create()
	{
		_effect.create(_vertexShader.c_str(), _pixelShader.c_str());
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

}
