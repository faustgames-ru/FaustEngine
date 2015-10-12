#include "ContentProvider.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>

namespace resources
{
#ifdef WIN32
	FILE * _file;

	bool ContentProvider::existContent(const char *name) 
	{
		return GetFileAttributesA(name) != INVALID_FILE_ATTRIBUTES;
	}
	
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

#endif /*WIN32*/
}