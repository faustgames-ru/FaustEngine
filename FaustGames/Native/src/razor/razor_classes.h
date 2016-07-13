#ifndef RAZOR_CLASSES_H
#define RAZOR_CLASSES_H

#include "razor_platform.h"
#include "razor_references.h"

namespace razor
{
	struct SpriteRVertex
	{
	public:
		core::Vector3 xyz;
		uint color;
		ushort u;
		ushort v;
	};
}

#endif /*RAZOR_CLASSES_H*/
