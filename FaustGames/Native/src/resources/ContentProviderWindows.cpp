#include "ContentProvider.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>

namespace resources
{
#ifdef WIN32
	FILE * _file;
	
	class WindowsContentProvider: public IAbstractContentProvider
	{
	public:
		static WindowsContentProvider Default;
		virtual bool existsContent(const char *name) override
		{
			return GetFileAttributesA(name) != INVALID_FILE_ATTRIBUTES;			
		}
		
		virtual void openContent(const char *name) override
		{
			_file = fopen(name, "rb");
		}

		virtual int read(void *buffer, int bytesLimit) override
		{
			return fread(buffer, 1, bytesLimit, _file);
		}
		
		virtual void closeContent() override
		{
			fclose(_file);
		}

		virtual int getContentSize() override
		{
			return 0;
		}
	};

	WindowsContentProvider WindowsContentProvider::Default;

	IAbstractContentProvider* ContentProvider::ContentProviderInstance(&WindowsContentProvider::Default);

	bool ContentProvider::existContent(const char *name) 
	{
		return ContentProviderInstance->existsContent(name);
	}
	
	void ContentProvider::openContent(const char *name)
	{
		ContentProviderInstance->openContent(name);
	}

	int ContentProvider::read(void *buffer, int bytesLimit)
	{
		return ContentProviderInstance->read(buffer, bytesLimit);
	}

	void ContentProvider::closeContent()
	{
		ContentProviderInstance->closeContent();
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