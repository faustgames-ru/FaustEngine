#ifndef CONTENT_PROVIDER_H
#define CONTENT_PROVIDER_H

#include "resources_classes.h"

namespace resources
{
	class ContentProvider
	{
	public:
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
}

#endif /*CONTENT_PROVIDER_H*/

