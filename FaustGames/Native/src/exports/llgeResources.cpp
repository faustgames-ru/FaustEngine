
#include "llge.h"

#include "../resources/ContentManager.h"
#include "../resources/ContentProvider.h"

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
			resources::ObbContentProvider::openObbFile(obbFile);
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


	extern "C" DLLEXPORT  IContentManager * API_CALL createContentManager()
	{
		return &resources::ContentManager::Default;
	}

	extern "C" DLLEXPORT IObbContentProvider * API_CALL createContentProvider()
	{
		return new ObbContentProvider();
	}

}