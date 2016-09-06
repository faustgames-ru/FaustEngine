#ifndef ZOMBO_CONTENT_SAVER_H
#define ZOMBO_CONTENT_SAVER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboContentSaver
	{
	public:
		static ZomboContentSaver Default;
		void setRoot(const std::string &rootPath);
		void rewrite(const std::string &fileName, const void *buffer, uint bytesCount);
	private:
		std::string _rootPath;
	};

	class SaverContentProvider
	{
	public:
		static void openContent(const char *name);
		static int write(const void *buffer, int bytesLimit);
		static void closeContent();
	private:
	};

}

#endif /*ZOMBO_CONTENT_SAVER_H*/