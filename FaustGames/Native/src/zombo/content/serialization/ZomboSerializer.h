#ifndef ZOMBO_SERIALIZER_H
#define ZOMBO_SERIALIZER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboSerializer
	{
	public:
		static ZomboValue * deserializeFromJson(const char* jsonString);
	private:
	};
}

#endif /*ZOMBO_SERIALIZER_H*/