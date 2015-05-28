#include "ContentProvider.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>

namespace resources
{

	FILE * _obbFile;
	struct ObbEntry
	{
		long Position;
		long Size;
		ObbEntry(){}
		ObbEntry(long position, long size)
		{
			Position = position;
			Size = size;
		}
	};

	std::map<std::string, ObbEntry> _entries;

	void ObbContentProvider::openObbFile(const char *obbFile)
	{
		_obbFile = fopen(obbFile, "rb");
		int count;
		fread(&count, 1, 4, _obbFile);
		if (count > 0)
		{
			char *buffer = new char[count * 256];
			fread(buffer, 1, count * 256, _obbFile);
			char *data = buffer;
			for (int i = 0; i < count; i++)
			{
				char nameLen = *((char *)data);
				data++;
				char * name = (char *)(data);
				data += 239;
				long position = *(long *)data;
				data += 8;
				long size = *(long *)data;
				data += 8;
				_entries[name] = ObbEntry(position, size);
			}
			delete[] buffer;
		}
	}

	void ObbContentProvider::closeObbFile()
	{
		fclose(_obbFile);
	}

	ObbEntry _currentEntry;

	bool ObbContentProvider::existsContent(const char *name)
	{
		std::string replace = name;
//#ifdef __ANDROID__
		for (int i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '\\')
				replace[i] = '_';
			if (replace[i] == '/')
				replace[i] = '_';
		}
//#endif
		std::map<std::string, ObbEntry>::iterator it = _entries.find(replace);
		return it != _entries.end();
	}
	
	void ObbContentProvider::openContent(const char *name)
	{
		std::string replace = name;
		//#ifdef __ANDROID__
		for (int i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '\\')
				replace[i] = '_';
			if (replace[i] == '/')
				replace[i] = '_';
		}
		//#endif
		_currentEntry = _entries[replace.c_str()];
		fsetpos(_obbFile, (fpos_t *)&_currentEntry.Position);
	}

	int ObbContentProvider::read(void *buffer, int bytesLimit)
	{
		return fread(buffer, 1, bytesLimit, _obbFile);
	}

	int ObbContentProvider::getContentSize()
	{
		return (int)_currentEntry.Size;
	}

	void ObbContentProvider::closeContent()
	{
	}


#ifdef __ANDROID__
	void ContentProvider::openContent(const char *name)
	{
		ObbContentProvider::openContent(name);
	}

	int ContentProvider::read(void *buffer, int bytesLimit)
	{
		ObbContentProvider::read(buffer, bytesLimit);
	}

	void ContentProvider::closeContent()
	{
		ObbContentProvider::closeContent();
	}
#endif /*__ANDROID__*/

}