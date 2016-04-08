#include "ContentProvider.h"
#include <string>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <map>

#ifdef __ANDROID__
#define nullptr 0

namespace resources
{
	AAssetManager* _assetsManager(nullptr);
	AAsset* _asset(nullptr);
	int64_t _size;
	int _readPointer;
	std::map<std::string, int64_t> _assetsEntries;
	char *_assetsBuffer(nullptr);

	void AssetsContentProvider::refresh(void* env, void* assetsManager)
	{
		_assetsManager = AAssetManager_fromJava(static_cast<JNIEnv *>(env), static_cast<jobject>(assetsManager));
	}

	void AssetsContentProvider::setup(void* env, void* assetsManager, const char* manifestFile)
	{
		_assetsManager = AAssetManager_fromJava(static_cast<JNIEnv *>(env), static_cast<jobject>(assetsManager));
		_asset = AAssetManager_open(_assetsManager, manifestFile, AASSET_MODE_UNKNOWN);
		int32_t count;
		AAsset_read(_asset, &count, 4);
		if (count > 0)
		{
			for (int i = 0; i < count; i++)
			{
				int64_t size;
				AAsset_read(_asset, &size, 8);
				int32_t len;
				AAsset_read(_asset, &len, 4);
				char *str = new char[len + 1];
				AAsset_read(_asset, str, len);
				str[len] = 0;
				_assetsEntries[str] = size;
				delete[] str;
			}
		}
		AAsset_close(_asset);
		_asset = nullptr;
	}

	bool AssetsContentProvider::existContent(const char *name)
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
		std::map<std::string, int64_t>::iterator it = _assetsEntries.find(replace);
		return it != _assetsEntries.end();
	}
	
	void AssetsContentProvider::openContent(const char *name)
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
		std::map<std::string, int64_t>::iterator it = _assetsEntries.find(replace);
		if (it == _assetsEntries.end())
		{
			_size = 0;
			_assetsBuffer = nullptr;
			return;
		}
		else
		{
			_size = it->second;
		}
		_asset = AAssetManager_open(_assetsManager, replace.c_str(), AASSET_MODE_UNKNOWN);
		_assetsBuffer = static_cast<char *>(malloc(_size));
		char *ptr = _assetsBuffer;
		int sizeLeft = _size;
		while (true)
		{
			int size = AAsset_read(_asset, ptr, sizeLeft);
			sizeLeft -= size;
			ptr += size;
			if (size == 0)
				break;
			if (sizeLeft == 0)
				break;
		}
		_readPointer = 0;
		AAsset_close(_asset);
		_asset = nullptr;
	}

	int AssetsContentProvider::read(void *buffer, int bytesLimit)
	{
		if (_assetsBuffer == nullptr) return 0;
		int leftsSize = _size - _readPointer;
		if (bytesLimit > leftsSize)
			bytesLimit = leftsSize;
		memcpy(buffer, _assetsBuffer + _readPointer, bytesLimit);
		_readPointer += bytesLimit;
		//_assetsBuffer 
		//return AAsset_read(_asset, buffer, bytesLimit);
		return bytesLimit;
	}

	int AssetsContentProvider::getContentSize()
	{
		return _size;
	}

	void AssetsContentProvider::closeContent()
	{
		if (_assetsBuffer == nullptr) return;
		free(_assetsBuffer);
	}
}
#endif
