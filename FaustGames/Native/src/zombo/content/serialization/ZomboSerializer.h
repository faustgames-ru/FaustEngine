#ifndef ZOMBO_SERIALIZER_H
#define ZOMBO_SERIALIZER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboSerializer
	{
	public:
		static ZomboValue * deserializeFromJson(const char* jsonString);
		static std::string serializeToJson(ZomboValue *value);
	private:
	};
}

#endif /*ZOMBO_SERIALIZER_H*/