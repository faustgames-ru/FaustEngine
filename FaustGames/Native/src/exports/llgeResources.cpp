
#include "llge.h"

#include "../resources/ContentManager.h"
#include "../resources/ContentProvider.h"
#include "../resources/TexturesManager.h"

namespace llge
{
	class ObbContentProvider : public IObbContentProvider
	{
	public:
		resources::IAbstractContentProvider* _provider;
		resources::IAbstractContentProvider* _obb;
		resources::IAbstractContentProvider* _assets;

		ObbContentProvider(): _provider(nullptr)
		{
			_obb = new resources::AndroidObbContentProvider();
			_assets = new resources::AndroidAssetsContentProvider();
		}

		virtual void API_CALL refreshAssetsManager(void *jniEnv, void *assetsManager)
		{			
			resources::AssetsContentProvider::refresh(jniEnv, assetsManager);
		}
			

		virtual void API_CALL openAssets(void *jniEnv, void *assetsManager)
		{
			resources::AssetsContentProvider::setup(jniEnv, assetsManager, "manifest.manifest");
			resources::ContentProvider::ContentProviderInstance = _provider = _assets;
		}


		virtual void API_CALL openObbFile(const char *obbFile)
		{
			resources::ObbContentProvider::openObbFile(obbFile, false);
			resources::ContentProvider::ContentProviderInstance = _provider = _obb;
		}

		virtual void API_CALL remapObbFile(const char *obbFile)
		{
			resources::ObbContentProvider::openObbFile(obbFile, true);
			resources::ContentProvider::ContentProviderInstance = _provider = _obb;
		}

		virtual void API_CALL closeObbFile()
		{
			resources::ObbContentProvider::closeObbFile();
		}

		virtual bool API_CALL existsContent(const char *name)
		{
			return _provider->existsContent(name);
		}

		virtual void API_CALL openContent(const char *name)
		{
			_provider->openContent(name);
		}
		virtual int API_CALL read(void *buffer, int bytesLimit)
		{
			return _provider->read(buffer, bytesLimit);
		}
		
		virtual int API_CALL getContentSize()
		{
			return _provider->getContentSize();
		}

		virtual void API_CALL closeContent()
		{
			_provider->closeContent();
		}
	};

	class ContentProviderExport : public llge::IContentProvider
	{
	public:
		static ContentProviderExport Deafualt;

		virtual bool API_CALL existsContent(const char *name) override
		{
			return resources::ContentProvider::existContent(name);
		}
		virtual void API_CALL openContent(const char *name) override
		{
			resources::ContentProvider::openContent(name);
		}
		virtual int API_CALL read(void *buffer, int bytesLimit) override
		{
			return resources::ContentProvider::read(buffer, bytesLimit);
		}
		virtual void API_CALL closeContent() override
		{
			resources::ContentProvider::closeContent();
		}
	};

	ContentProviderExport ContentProviderExport::Deafualt;

	extern "C" DLLEXPORT  IContentManager * API_CALL createContentManager()
	{
		return &resources::ContentManager::Default;
	}

	extern "C" DLLEXPORT void API_CALL setReplaceSeparator(bool value)
	{
		resources::ContentProvider::ReplaceSeparator = value;
	}

	extern "C" DLLEXPORT void API_CALL useCompression(TextureImage2dFormat format)
	{
		resources::TexturesLoader::Default.useCompression(format);
		resources::ContentManager::Default.useCompression(format);
	}

	extern "C" DLLEXPORT IContentProvider * API_CALL getContentProvider()
	{
		return &ContentProviderExport::Deafualt;
	}

	
	extern "C" DLLEXPORT ITexturesManager * API_CALL createTexturesManager()
	{
		return new resources::TexturesManager(&resources::TexturesLoader::Default);
	}
	
	extern "C" DLLEXPORT IObbContentProvider * API_CALL createContentProvider()
	{
		return new ObbContentProvider();
	}
}
