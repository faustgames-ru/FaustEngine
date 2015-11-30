#ifndef PATH_H
#define PATH_H

#include "core_classes.h"

namespace core
{
	class Path
	{
	public:
		static std::string emptyString;
		static std::string getFileExt(const std::string &fileName);
		static std::string getFilePath(const std::string &fileName);
	};
}

#endif /*PATH_H*/