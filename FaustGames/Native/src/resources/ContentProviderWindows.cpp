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


	void AssetsContentProvider::setup(void* env, void* assetsManager, const char* manifestFile)
	{
	}

	void AssetsContentProvider::refresh(void* env, void* assetsManager)
	{
	}

	bool AssetsContentProvider::existContent(const char* name)
	{
		return false;
	}

	void AssetsContentProvider::openContent(const char* name)
	{
	}

	int AssetsContentProvider::read(void* buffer, int bytesLimit)
	{
		return 0;
	}

	int AssetsContentProvider::getContentSize()
	{
		return 0;
	}

	void AssetsContentProvider::closeContent()
	{
	}

#endif /*WIN32*/
}