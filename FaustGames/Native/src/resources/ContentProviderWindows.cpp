#include "ContentProvider.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>

namespace resources
{
#ifdef WIN32
	FILE * _file;
	int _fileSize;

	long GetFileSize(const char* filename)
	{
		struct stat stat_buf;
		int rc = stat(filename, &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	}


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
			_fileSize = GetFileSize(name);
			_file = fopen(name, "rb");
			auto err = ferror(_file);
		}

		virtual int read(void *buffer, int bytesLimit) override
		{
			int result = fread(buffer, 1, bytesLimit, _file);
			auto err = ferror(_file);
			return result;
		}
		
		virtual void closeContent() override
		{
			fclose(_file);
			auto err = ferror(_file);
			_fileSize = 0;
		}

		virtual int getContentSize() override
		{
			return _fileSize;
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
		
	bool ContentProvider::ReplaceSeparator(false);

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