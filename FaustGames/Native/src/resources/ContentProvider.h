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
}

#endif /*CONTENT_PROVIDER_H*/

