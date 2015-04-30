
#include "llge.h"

#include "../resources/ContentManager.h"
#include "../resources/ContentProvider.h"

namespace llge
{
	class ObbContentProvider : public IObbContentProvider
	{
	public:
		virtual void API_CALL openObbFile(const char *obbFile)
		{
			resources::ObbContentProvider::openObbFile(obbFile);
		}

		virtual void API_CALL closeObbFile()
		{
			resources::ObbContentProvider::closeObbFile();
		}

		virtual bool API_CALL existsContent(const char *name)
		{
			return resources::ObbContentProvider::existsContent(name);
		}

		virtual void API_CALL openContent(const char *name)
		{
			resources::ObbContentProvider::openContent(name);
		}
		virtual int API_CALL read(void *buffer, int bytesLimit)
		{
			return resources::ObbContentProvider::read(buffer, bytesLimit);
		}
		
		virtual int API_CALL getContentSize()
		{
			return resources::ObbContentProvider::getContentSize();
		}

		virtual void API_CALL closeContent()
		{
			resources::ObbContentProvider::closeContent();
		}
	};


	extern "C" DLLEXPORT  IContentManager * API_CALL createContentManager()
	{
		return new resources::ContentManager();
	}

	extern "C" DLLEXPORT IObbContentProvider * API_CALL createContentProvider()
	{
		return new ObbContentProvider();
	}

}