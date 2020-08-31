#include <jni.h>
#include <string>
#include "BitmapMatUtils.h"
#include <android/log.h>
#include "cardocr.h"
#include "vector"

using namespace co1;
using namespace std;


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
    find_card_number_area(cart_mat, card_number_area);

//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/cart_mat.jpg", cart_mat);
//    __android_log_print(ANDROID_LOG_WARN, "TAG", "cart_mat");


    vector<Mat> numbers;



    return env->NewStringUTF("622848");
}