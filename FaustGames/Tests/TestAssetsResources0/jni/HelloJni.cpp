#include <stdio.h>
#include <string.h>
#include <jni.h>

#include "../../../Native/src/resources/ContentProvider.h"
/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni_HelloJni_setup(JNIEnv* env, jobject thiz, jobject assetManager)
{
	resources::setAndroidAssetsManager(env, assetManager);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env, jobject thiz )
{

	resources::ContentProvider::openContent("test.txt");
	int l = 1024;
	char *string = new char[l];
	resources::ContentProvider::read(string, l);
	string[l - 1] = 0;
	resources::ContentProvider::closeContent();
	jstring result = env->NewStringUTF(string);
	delete[] string;
	return result;
}
