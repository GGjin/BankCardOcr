//
// Created by GG on 2020/8/26.
//

#ifndef BANKCARDOCR_BITMAPMATUTILS_H
#define BANKCARDOCR_BITMAPMATUTILS_H

#include <jni.h>
#include "opencv2/opencv.hpp"

using namespace cv;

class BitmapMatUtils {

public:
    static int bitmap2mat(JNIEnv *env, jobject bitmap, Mat& mat);

};


#endif //BANKCARDOCR_BITMAPMATUTILS_H
