#include "ZomboContentSaver.h"

namespace zombo
{
	ZomboContentSaver ZomboContentSaver::Default;

	void ZomboContentSaver::setRoot(const std::string& rootPath)
	{
		_rootPath = rootPath;
	}

	void ZomboContentSaver::rewrite(const std::string& fileName, const void* buffer, uint bytesCount)
	{
		SaverContentProvider::openContent((_rootPath + fileName).c_str());
		SaverContentProvider::write(buffer, bytesCount);
		SaverContentProvider::closeContent();
	}

	FILE * _file;

	void SaverContentProvider::openContent(const char* name)
	{
		_file = fopen(name, "w+");
	}

	int SaverContentProvider::write(const void* buffer, int bytesLimit)
	{
		return fwrite(buffer, 1, bytesLimit, _file);
	}

	void SaverContentProvider::closeContent()
	{
		fclose(_file);
	}
}
