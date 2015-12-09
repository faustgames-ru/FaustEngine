#ifndef ZOMBO_SERIALIZER_H
#define ZOMBO_SERIALIZER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboSerializer
	{
	public:
		ZomboSerializer();
		~ZomboSerializer();
		ZomboObject *getRoot() const;
		static ZomboSerializer* createFromJson(const char* jsonString);
		void dispose() const;
	private:
		ZomboSerializer(ZomboObject *root);
		ZomboObject *_root;
	};
}

#endif /*ZOMBO_SERIALIZER_H*/