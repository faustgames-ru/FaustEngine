#ifndef CONTENT_PROVIDER_H
#define CONTENT_PROVIDER_H

#include "resources_classes.h"

namespace resources
{

	class IAbstractContentProvider : public IBaseObject
	{
	public:
		virtual bool existsContent(const char *name) = 0;
		virtual void openContent(const char *name) = 0;
		virtual int read(void *buffer, int bytesLimit) = 0;
		virtual int getContentSize() = 0;
		virtual void closeContent() = 0;
	};

	class ContentProvider
	{
	public:
		static IAbstractContentProvider* ContentProviderInstance;
		static bool existContent(const char *name);
		static void openContent(const char *name);
		static int read(void *buffer, int bytesLimit);
		static void closeContent();
	private:
	};

	class ObbContentProvider
	{
	public:
		static void openObbFile(const char *obbFile);
		static void closeObbFile();

        static bool existsContent(const char *name);
		static void openContent(const char *name);
		static int read(void *buffer, int bytesLimit);
		static int getContentSize();
		static void closeContent();
	};

	class AssetsContentProvider
	{
	public:
		static void setup(void *env, void* assetsManager, const char *manifestFile);
		static void refresh(void *env, void* assetsManager);
		static bool existContent(const char *name);
		static void openContent(const char *name);
		static int read(void *buffer, int bytesLimit);
		static int getContentSize();
		static void closeContent();
	};

	class AndroidAssetsContentProvider : public IAbstractContentProvider
	{
	public:
		virtual bool existsContent(const char *name) OVERRIDE
		{
			return AssetsContentProvider::existContent(name);
		}
		virtual void openContent(const char *name) OVERRIDE
		{
			AssetsContentProvider::openContent(name);
		}
		virtual int read(void *buffer, int bytesLimit) OVERRIDE
		{
			return AssetsContentProvider::read(buffer, bytesLimit);
		}
		virtual int getContentSize() OVERRIDE		
		{
			return AssetsContentProvider::getContentSize();
		}

		virtual void closeContent() OVERRIDE
		{
			AssetsContentProvider::closeContent();
		}
	};


	class AndroidObbContentProvider : public IAbstractContentProvider
	{
	public:
		virtual bool existsContent(const char* name) OVERRIDE
		{
			return ObbContentProvider::existsContent(name);
		}

		virtual void openContent(const char* name) OVERRIDE
		{
			ObbContentProvider::openContent(name);
		}

		virtual int read(void *buffer, int bytesLimit) OVERRIDE
		{
			return ObbContentProvider::read(buffer, bytesLimit);
		}

		virtual int getContentSize() OVERRIDE
		{
			return ObbContentProvider::getContentSize();
		}

		virtual void closeContent() OVERRIDE
		{
			ObbContentProvider::closeContent();
		}
	};

}

#endif /*CONTENT_PROVIDER_H*/

