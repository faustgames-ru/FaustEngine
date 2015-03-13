#include "ContentProvider.h"
#include <stdio.h>
#include <stdlib.h>

namespace resources
{
	FILE * _file;

	void ContentProvider::openContent(const char *name)
	{
		_file = fopen(name, "rb");
	}
	
	int ContentProvider::read(void *buffer, int bytesLimit)
	{
		return fread(buffer, 1, bytesLimit, _file);
	}
	
	void ContentProvider::closeContent()
	{
		fclose(_file);
	}
}