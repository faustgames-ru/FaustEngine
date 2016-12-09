#include <fcntl.h>
#include <sys/mman.h>
#include "ContentProvider.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <map>
#include <cstdint>

#ifdef __APPLE__
#endif

namespace resources
{

    FILE * _obbFile = nullptr;
    void * _mapAddress = nullptr;
    int64_t _readCur = 0;
    int64_t _mapSize = 0;
    int64_t _mapCursor = 0;
    int _obbFd = 0;
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
        _mapSize = 0;
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
                
                if (_mapSize < (position + size))
                    _mapSize = position + size;
			}
			delete[] buffer;
		}
        fclose(_obbFile);
        /*
        _obbFile = fopen(obbFile, "rb");
        _mapAddress = new char[_mapSize];
        fread(_mapAddress, 1, (int)_mapSize, _obbFile);
        fclose(_obbFile);
        */
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
        
        if (_obbFile == nullptr)
        {
            _obbFile = fopen(_obbPath.c_str(), "rb");
        }
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
        //int status = fseek(_obbFile, _currentEntry.Position, SEEK_CUR);
        _readCur = _currentEntry.Position;
        fseek(_obbFile, _currentEntry.Position, SEEK_SET);
	}

    int ObbContentProvider::read(void *buffer, int bytesLimit)
    {
        int64_t entryFinishPos = _currentEntry.Position + _currentEntry.Size;
        int64_t finishPos = _readCur + bytesLimit;
        
        if (finishPos > entryFinishPos)
        {
            bytesLimit -= finishPos - entryFinishPos;
        }
        bytesLimit = fread(buffer, 1, bytesLimit, _obbFile);
        _readCur += bytesLimit;
        return bytesLimit;
    }
    
    void ObbContentProvider::closeContent()
    {
        fclose(_obbFile);
        _obbFile = nullptr;
    }
 /*
    
    void ObbContentProvider::openContent(const char *name)
    {
        if (!existsContent(name))
        {
            return;
        }
        
        if (_mapAddress == nullptr)
        {
            _obbFd = open(_obbPath.c_str(), O_RDONLY);
            _mapAddress = mmap(nullptr, (int)_mapSize, PROT_READ, MAP_SHARED, _obbFd, 0);
        }
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
        _mapCursor = _currentEntry.Position;
    }
    
    int ObbContentProvider::read(void *buffer, int bytesLimit)
    {
        int64_t entryFinishPos = _currentEntry.Position + _currentEntry.Size;
        int64_t finishPos = _mapCursor + bytesLimit;
        
        if (finishPos > entryFinishPos)
        {
            bytesLimit -= finishPos - entryFinishPos;
        }
        
        char* src = ((char* )_mapAddress) +_mapCursor;
        
        memcpy(buffer, src, bytesLimit);
        _mapCursor += bytesLimit;
        return bytesLimit;
    }
     
     
    void ObbContentProvider::closeContent()
    {
    }
     
*/
    int ObbContentProvider::getContentSize()
	{
		return (int)_currentEntry.Size;
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
    
    void AssetsContentProvider::setup(void *env, void* assetsManager, const char *manifestFile)
    {
    }

    void AssetsContentProvider::refresh(void *env, void* assetsManager)
    {
    }
    
    bool AssetsContentProvider::existContent(const char *name)
    {
         return false;
    }
    
    void AssetsContentProvider::openContent(const char *name)
    {
    }
    
    int AssetsContentProvider::read(void *buffer, int bytesLimit)
    {
        return 0;
    }
    
    int AssetsContentProvider::getContentSize()
    {
         return 0;
    }
    
    void AssetsContentProvider::closeContent()
    {
    }
#endif

}
