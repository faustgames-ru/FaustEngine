#include "ContentProvider.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <map>

#ifdef __APPLE__
#endif

namespace resources
{

	FILE * _obbFile;
	struct ObbEntry
	{
		int64_t Position;
		int64_t Size;
		ObbEntry(){}
		ObbEntry(int64_t position, int64_t size)
		{
			Position = position;
			Size = size;
		}
	};

    /*
    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    */
	std::map<std::string, ObbEntry> _entries;
    std::string _obbPath;
	void ObbContentProvider::openObbFile(const char *obbFile)
	{
        _obbPath = obbFile;
		_obbFile = fopen(obbFile, "rb");
		int32_t count;
        
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
				int64_t  position = *(int64_t *)data;
                data += 8;
				int64_t size = *(int64_t *)data;
				data += 8;
				_entries[name] = ObbEntry(position, size);
			}
			delete[] buffer;
		}
        fclose(_obbFile);
        _obbFile = 0;
	}

	void ObbContentProvider::closeObbFile()
	{
	}

	ObbEntry _currentEntry;

	bool ObbContentProvider::existsContent(const char *name)
	{
		std::string replace = name;
//#ifdef __ANDROID__
		for (uint i = 0; i < replace.size(); i++)
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
        if (!existsContent(name))
        {
            return;
        }
        _obbFile = fopen(_obbPath.c_str(), "rb");
		std::string replace = name;
		//#ifdef __ANDROID__
		for (uint i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '\\')
				replace[i] = '_';
			if (replace[i] == '/')
				replace[i] = '_';
		}
		//#endif

		_currentEntry = _entries[replace.c_str()];
        int status = fseek(_obbFile, _currentEntry.Position, SEEK_CUR);
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
        fclose(_obbFile);
	}


	IAndroidContentProvider* ContentProvider::AndroidContentProvider(nullptr);

#ifdef __ANDROID__

	bool ContentProvider::existContent(const char *name)	
	{
		return AndroidContentProvider->existsContent(name);
	}

	void ContentProvider::openContent(const char *name)
	{
		AndroidContentProvider->openContent(name);
	}

	int ContentProvider::read(void *buffer, int bytesLimit)
	{
		return AndroidContentProvider->read(buffer, bytesLimit);
	}

	void ContentProvider::closeContent()
	{
		AndroidContentProvider->closeContent();
	}
#endif /*__ANDROID__*/
    
#ifdef __APPLE__
	bool ContentProvider::existContent(const char *name)
	{
		return ObbContentProvider::existsContent(name);
	}
	
    void ContentProvider::openContent(const char *name)
    {
        ObbContentProvider::openContent(name);
    }
    
    int ContentProvider::read(void *buffer, int bytesLimit)
    {
        return ObbContentProvider::read(buffer, bytesLimit);
    }
    
    void ContentProvider::closeContent()
    {
        ObbContentProvider::closeContent();
    }
#endif

}