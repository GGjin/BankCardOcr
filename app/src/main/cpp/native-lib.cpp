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
    //对图像初步处理
    find_card_ocr(mat, card_area);
    //裁剪并获取银行卡区域
    Mat cart_mat(mat, card_area);
    //3. 裁剪银行卡号区域
    Rect card_number_area;

    find_card_number_area(mat, card_number_area);


    return env->NewStringUTF("622848");
}