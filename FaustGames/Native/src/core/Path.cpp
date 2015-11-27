#include "Path.h"

namespace core
{
	std::string Path::emptyString = "";

	std::string Path::getFileExt(const std::string& fileName)
	{
		int max = fileName.size() - 1;
		int i = max;
		bool find = false;
		for (; i >= 0; i--)
		{
			if (fileName[i] == '.')
			{
				find = true;
				break;
			}
		}
		i++;
		if (!find)
			return emptyString;
		if (i == max)
			return emptyString;
		std::string result = fileName.substr(i, fileName.size() - i);
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}
}
