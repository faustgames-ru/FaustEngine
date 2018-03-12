#include <fcntl.h>
//#include <sys/mman.h>
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
	//void * _mapAddress = nullptr;
	
	int64_t _readCur = 0;
	int64_t _mapSize = 0;
	int64_t _mapCursor = 0;
	int _obbFd = 0;
	struct ObbEntry
	{
		int64_t Position;
		int64_t Size;
		int FileIndex;
		ObbEntry() {}
		ObbEntry(int64_t position, int64_t size, int fileIndex)
		{
			Position = position;
			Size = size;
			FileIndex = fileIndex;
		}
	};

	std::map<std::string, ObbEntry> _entries;
	std::vector<std::string> _obbFiles;
	//std::string _obbPath;

	int findObbFile(const std::string &obbFileName)
	{
		for (int i = 0; i < _obbFiles.size(); i++)
		{
			if (obbFileName == _obbFiles[i])
				return i;
		}
		int result = _obbFiles.size();
		_obbFiles.push_back(obbFileName);
		return -1;
	}

	void ObbContentProvider::openObbFile(const char *obbFile, bool remap)
	{		
		int obbIndex = findObbFile(obbFile);

		if (obbIndex < 0) 
		{
			obbIndex = _obbFiles.size();
			_obbFiles.push_back(obbFile);
		}
		else
		{
			if (!remap) return;
		}
		//_obbPath = obbFile;
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
				_entries[name] = ObbEntry(position, size, obbIndex);
                
                if (_mapSize < (position + size))
                    _mapSize = position + size;
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
        
        //if (_obbFile == nullptr)
        //{
        //    _obbFile = fopen(_obbPath.c_str(), "rb");
        //}

        std::string replace = name;
		for (uint i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '\\')
				replace[i] = '_';
			if (replace[i] == '/')
				replace[i] = '_';
		}
		
		_currentEntry = _entries[replace.c_str()];
        _readCur = _currentEntry.Position;		
		_obbFile = fopen(_obbFiles[_currentEntry.FileIndex].c_str(), "rb");		
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

    int ObbContentProvider::getContentSize()
	{
		return (int)_currentEntry.Size;
	}

#ifdef __ANDROID__
    bool ContentProvider::ReplaceSeparator(false);
	IAbstractContentProvider* ContentProvider::ContentProviderInstance(nullptr);

	bool ContentProvider::existContent(const char *name)
	{
		return ContentProviderInstance->existsContent(name);
	}

	void ContentProvider::openContent(const char *name)
	{
		ContentProviderInstance->openContent(name);
	}

	int ContentProvider::read(void *buffer, int bytesLimit)
	{
		return ContentProviderInstance->read(buffer, bytesLimit);
	}

	void ContentProvider::closeContent()
	{
		ContentProviderInstance->closeContent();
	}
#endif /*__ANDROID__*/
    
#ifdef __APPLE__
    bool ContentProvider::ReplaceSeparator(false);
	IAbstractContentProvider* ContentProvider::ContentProviderInstance(nullptr);


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
