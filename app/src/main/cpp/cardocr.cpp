//
// Created by GG on 2020/8/26.
//

#include "cardocr.h"
#include <vector>
#include <android/log.h>

using namespace std;

/**
 * 裁剪银行卡图片，获取到银行卡的轮廓
 * @param mat
 * @param area
 * @return
 */
int co1::find_card_ocr(const Mat &mat, Rect &area) {

    //1 先降噪,

    Mat blur;
    //使用高斯滤镜模糊图像。可以指定高斯内核进行卷积
    GaussianBlur(mat, blur, Size(5, 5), BORDER_DEFAULT, BORDER_DEFAULT);
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/blur.jpg", blur);
//    __android_log_print(ANDROID_LOG_WARN, "TAG", "blur");

    //2 边缘梯度增强，x y 方向上增强， 并保存图片看看
    Mat gard_x, gard_y;
    //使用Scharr运算符计算第一个x或者y图像导数
    Scharr(blur, gard_x, CV_32F, 1, 0);
    Scharr(blur, gard_y, CV_32F, 0, 1);

    Mat abs_gard_x, abs_gard_y;
    //缩放，取绝对值，将结果转换为8位的函数
    convertScaleAbs(gard_x, abs_gard_x);
    convertScaleAbs(gard_y, abs_gard_y);

    Mat gard;
    //计算两个数组的加权和
    addWeighted(abs_gard_x, 0.5, abs_gard_y, 0.5, 0, gard);

//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/gard.jpg", gard);
//    __android_log_print(ANDROID_LOG_WARN, "TAG", "gard");

    //3 这一步讲图片转换为灰度图片
    Mat grey;
    //将图像从一种颜色空间转为另一种颜色空间
    cvtColor(gard, grey, COLOR_BGRA2GRAY);
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/grey.jpg", grey);
//    __android_log_print(ANDROID_LOG_WARN, "TAG", "grey");
    //4 二值化，筛选轮廓
    Mat binary;
    threshold(grey, binary, 40, 255, THRESH_BINARY);
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/binary.jpg", binary);
//    __android_log_print(ANDROID_LOG_WARN, "TAG", "binary");

    //5 截图图片中银行卡的轮廓位置
    vector<vector<Point>> contours;
    //在二进制图像中查找轮廓
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); i++) {
        //这个函数会计算并返回，指定的像素或者点集的垂直矩阵图像，简单来说就是把无用的边切割掉，将需要的图像切割一下返回最小的轮廓
        Rect rect = boundingRect(contours[i]);
        if (rect.width > mat.cols / 2 && rect.height > mat.rows / 2 && rect.width != mat.cols) {
            area = rect;
            break;
        }
    }


    blur.release();
    gard_x.release();
    gard_y.release();
    abs_gard_x.release();
    abs_gard_y.release();
    gard.release();
    grey.release();
    binary.release();

    return 0;
}

int co1::find_card_number_area(const Mat &mat, Rect &area) {
    // 有两种方式，一种是精确截取，找到银联区域通过大小比例精确的截取 （70%）
    // 粗略的截取，截取高度 1/2 - 3/4 ， 宽度 1/12  11/12    90%  （采用）
    // 万一找不到 ，可以手动的输入和修改

    area.x = mat.cols / 12;
    area.y = mat.rows / 2;

    area.width = mat.rows * 5 / 6;
    area.height = mat.rows / 4;

    return 0;
}