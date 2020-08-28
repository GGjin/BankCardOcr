//
// Created by GG on 2020/8/26.
//

#include "cardocr.h"
#include <vector>
#include <android/log.h>

using namespace std;

int co1::find_card_ocr(const Mat &mat, Rect &area) {

    //先降噪,

    Mat blur;
    GaussianBlur(mat, blur, Size(5, 5), BORDER_DEFAULT, BORDER_DEFAULT);
    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/blur.jpg", blur);
    __android_log_print(ANDROID_LOG_WARN, "TAG", "blur");

    return 0;
}