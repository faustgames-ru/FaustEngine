#ifndef UNIFORM_H
#define UNIFORM_H

#include "graphics.h"

namespace graphics
{
	class Sampler
	{
	public:
		void applyTexture(GLuint textureHandler);
	protected:
	private:
		unsigned int _mask;
		GLuint _index;
		GLuint _textureHandle;
	};
}

#endif /*UNIFORM_H*/
