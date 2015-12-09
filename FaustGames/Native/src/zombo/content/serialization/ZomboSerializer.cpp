#include "ZomboSerializer.h"
#include "ZomboObject.h"

namespace zombo
{
	ZomboSerializer::ZomboSerializer(): _root(nullptr)
	{

	}

	ZomboSerializer::~ZomboSerializer()
	{
		if (_root != nullptr)
			delete _root;
	}

	ZomboObject* ZomboSerializer::getRoot() const
	{
		return _root;
	}

	ZomboSerializer* ZomboSerializer::createFromJson(const char* jsonString)
	{
		// todo: parse jsonString
		return new ZomboSerializer();
	}

	void ZomboSerializer::dispose() const
	{
		delete this;
	}

	ZomboSerializer::ZomboSerializer(ZomboObject* root): _root(root)
	{
	}
}
