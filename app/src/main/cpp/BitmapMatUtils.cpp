//
// Created by GG on 2020/8/26.
//

#include "BitmapMatUtils.h"
#include <android/bitmap.h>

int BitmapMatUtils::bitmap2mat(JNIEnv *env, jobject bitmap, Mat &mat) {


    //1.锁定画布
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    //构造 mat 对象 ，还要判断是什么颜色通道的 0-255
    //获取bitmap信息

    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);


    //返回三通道 CV_8UC4 -> argb CV_8UC2 -> rgb  CV_8UC1 -> 黑白
    Mat createMat(bitmapInfo.height, bitmapInfo.width, CV_8UC4);

    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {//mat 颜色通道里的四颜色通道 CV_8UC4
        Mat temp(bitmapInfo.height, bitmapInfo.width, CV_8UC4, pixels);
        temp.copyTo(createMat);
        temp.release();
    } else if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {//mat 三颜色通道 ， CV_8UC2
        Mat temp(bitmapInfo.height, bitmapInfo.width, CV_8UC2, pixels);
        //三颜色通道转为四颜色通道
        cvtColor(temp, createMat, COLOR_BGR5652BGRA);

        temp.release();
    }

    //拷贝图片到 矩阵中
    createMat.copyTo(mat);

    createMat.release();


    //解锁画布
    AndroidBitmap_unlockPixels(env, bitmap);
    return 0;
}

int BitmapMatUtils::mat2bitmap(JNIEnv *env, jobject bitmap, Mat &mat) {
    return 0;
}
