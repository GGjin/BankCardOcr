#include <jni.h>
#include <string>
#include "BitmapMatUtils.h"
#include <android/log.h>
#include "cardocr.h"

using namespace co1;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_gg_bankcardocr_BankCardOcr_carOcr(JNIEnv *env, jobject thiz, jobject bitmap) {

    //1. bitmap -> mat

    Mat mat;
    BitmapMatUtils::bitmap2mat(env, bitmap, mat);

    //2. 轮廓增强(梯度增强)
    Rect card_area;

    find_card_ocr(mat, card_area);

    return env->NewStringUTF("622848");
}