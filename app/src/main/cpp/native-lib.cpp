#include <jni.h>
#include <string>
#include "BitmapMatUtils.h"
#include <android/log.h>
#include "cardocr.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_gg_bankcardocr_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
