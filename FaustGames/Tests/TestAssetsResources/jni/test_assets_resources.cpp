/**********************************
 Java Native Interface library
**********************************/
#include <jni.h>
#include <android/log.h>

/** This is the C++ implementation of the Java native method.
@param env Pointer to JVM environment
@param thiz Reference to Java this object
*/
extern "C"
JNIEXPORT void JNICALL
Java_com_faustgames_test_assets_resources_test_assets_resources_test_assets_resourcesNative( JNIEnv* env, jobject thiz )
{
	// Enter code here
	
	__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "trolololo");

}
