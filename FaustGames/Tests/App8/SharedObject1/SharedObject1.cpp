#include "SharedObject1.h"
#include "../../../Native/src/resources/ContentProvider.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SharedObject1", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SharedObject1", __VA_ARGS__))

extern "C" {
	/* This trivial function returns the platform ABI for which this dynamic native library is compiled.*/
	const char * SharedObject1::getPlatformABI()
	{
	#if defined(__arm__)
	#if defined(__ARM_ARCH_7A__)
	#if defined(__ARM_NEON__)
		#define ABI "armeabi-v7a/NEON"
	#else
		#define ABI "armeabi-v7a"
	#endif
	#else
		#define ABI "armeabi"
	#endif
	#elif defined(__i386__)
		#define ABI "x86"
	#else
		#define ABI "unknown"
	#endif
		LOGI("This dynamic shared library is compiled with ABI: %s", ABI);
		return "This native library is compiled with ABI: %s" ABI ".";
	}

	void SharedObject1()
	{
	}

	SharedObject1::SharedObject1()
	{
	}

	SharedObject1::~SharedObject1()
	{
	}

	void setupAssetsManager(JNIEnv* env, jobject assetManager)
	{
		resources::AssetsContentProvider::setup(env, assetManager, "manifest.manifest");
	}

	void existsAssets(const char *assetsName)
	{
		resources::AssetsContentProvider::existContent(assetsName);
	}

	void openAssets(const char *assetsName)
	{
		resources::AssetsContentProvider::openContent(assetsName);
	}
	int readAssets(void *buffer, int bytesLimit)
	{
		return resources::AssetsContentProvider::read(buffer, bytesLimit);
	}
	void closeAssets()
	{
		return resources::AssetsContentProvider::closeContent();
	}
}

