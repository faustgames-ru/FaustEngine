#ifndef ZOMBO_SERIALIZER_H
#define ZOMBO_SERIALIZER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboSerializer
	{
	public:
		ZomboObject *getRoot();		
		
		static ZomboSerializer* createFromJson(const char* jsonString);
		void dispose();
	};
}

#endif /*ZOMBO_SERIALIZER_H*/