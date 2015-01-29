#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphics.h"
#include "Lazy.h"
#include "UniformInfo.h"

namespace graphics
{
	class Effects
	{
	private:
		static core::Lazy<Effects> _instance;
		static Effects * instanciate();
		static void deinstanciate(Effects *value);
		static std::string _solidPixelShader;
		static std::string _solidVertexShader;
		
		Effect * _solid;

		Effects();
		~Effects();
	public:
		static Effects *instance();
		void create();
		void cleanup();
		Effect * solid();
	};
}

#endif /*EFFECTS_H*/