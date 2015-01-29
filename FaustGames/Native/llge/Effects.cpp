#include "Effects.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "ObjectsPool.h"

namespace graphics
{
	Effects::Effects()
	{
		_solid = new Effect(_solidVertexShader.c_str(), _solidPixelShader.c_str());
		_solid->addUniform(Uniforms::instance()->projection());
	}
	

	void Effects::create()
	{
		_solid->create();
	}

	void Effects::cleanup()
	{
		_solid->cleanup();
	}


	Effect * Effects::solid()
	{
		return _solid;
	}


	Effects::~Effects()
	{
		delete _solid;
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

	std::string Effects::_solidPixelShader(
"\
void main() \
{\
gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\
}\
");

	std::string Effects::_solidVertexShader(
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