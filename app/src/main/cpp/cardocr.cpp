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

    area.width = mat.cols * 5 / 6;
    area.height = mat.rows / 4;

    return 0;
}


int co1::find_card_numbers(const Mat &mat, vector<Mat> numbers) {

    //1.灰度处理
    Mat gray;
    cvtColor(mat, gray, COLOR_BGR2GRAY);


    vector<Mat> splitBGR(gray.channels());
    //分割通道，存储到splitBGR中
    split(gray, splitBGR);
    //对各个通道分别进行直方图均衡化
    for (int i = 0; i < gray.channels(); i++)
        equalizeHist(splitBGR[i], splitBGR[i]);
    //合并通道
    merge(splitBGR, gray);

//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/number_gray_channel.jpg", gray);
//    //2.二值化 找到合适自己的值，这一步非常关键
    Mat binary;
    threshold(gray,binary, 40, 255, THRESH_BINARY );
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/number_binary.jpg", binary);


    //3.降噪处理

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(binary, binary, MORPH_CLOSE, kernel);
    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/kernel.jpg", kernel);

//
//    //4.取反白，  白黑->黑白
//
//    Mat binary_not = binary.clone();
//    bitwise_not(binary_not, binary_not);
//
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/binary_not.jpg", binary_not);
//
//    //5.查找轮廓
//    vector<vector<Point>> contours;
//    findContours(binary_not, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//    //6.去掉过多干扰项， 通过匹配数字的大小和轮廓信息
//    int mat_area = mat.rows * mat.cols;
//    int min_h = mat.rows / 4;
//
//    //通过匹配过小的轮廓和比较矮的轮廓，将其填充为黑色，来过滤掉图像
//    for (int i = 0; i < contours.size(); i++) {
//        Rect rect = boundingRect(contours[i]);
//        int area = rect.height * rect.width;
//
//        if (area < mat_area / 200) {
//            drawContours(binary_not, contours, i, Scalar(0), -1);
//        } else if (rect.height < min_h) {
//            drawContours(binary_not, contours, i, Scalar(0), -1);
//        }
//
//    }
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/binary_not_noise.jpg", binary_not);
//
////    // 截取每个数字的轮廓 binary(没噪音) 不行，binary_not(有噪音)
////    binary.copyTo(binary_not);
////    bitwise_not(binary_not, binary_not);// 没有噪音的 binary_not
//
//    contours.clear();
//
//    findContours(binary_not, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//    //先把Rect存起来，查找可能出现的循环错误，还有可能出现粘连字符
//    Rect rects[contours.size()];
//
//    //构造一个白色的图片，单颜色
//    Mat contours_mat(binary.size(), CV_8UC1, Scalar(255));
//
//    int min_w = mat.cols;
//
//    for (int i = 0; i < contours.size(); i++) {
//        rects[i] = boundingRect(contours[i]);
//
//        drawContours(contours_mat, contours, i, Scalar(0), 1);
//
//        min_w = min(rects[i].width, min_w);
//
//    }
//
//    imwrite("/storage/emulated/0/Android/data/com.gg.bankcardocr/contours_mat.jpg", contours_mat);
//
//    //7.进行排序，防止存储数据错位
//    for (int i = 0; i < contours.size(); ++i) {
//        for (int j = 0; j < contours.size() - i; ++j) {
//            if (rects[j].x < rects[j + 1].x) {
//                swap(rects[j], rects[j + 1]);
//
//            }
//        }
//
//    }
//
//    //8.开始裁剪图片， 因为原图片 mat 是彩色图片，数据量太大，所有将图片转为单通道的 并且像素少一点的图片
//    numbers.clear();
//
//    for (int i = 0; i < contours.size(); i++) {
//        if (rects[i].width >= min_w * 2) {
//            Mat mat(contours_mat, rects[i]);
//
//            int cols_pos = co1::find_split_cols_pos(mat);
//            //左右两个数字都存进去
//            Rect rect_left(0, 0, cols_pos - 1, mat.rows);
//            numbers.push_back(Mat(mat, rect_left));
//            Rect rect_right(cols_pos, 0, mat.cols, mat.rows);
//            numbers.push_back(Mat(mat, rect_right));
//
//        } else {
//            Mat number(contours_mat, rects[i]);
//
//            numbers.push_back(number);
//
//
//        }
//
//    }
//
//
//    //保存数字图片
//    for (int i = 0; i < numbers.size(); ++i) {
//        char name[60];
//        sprintf(name, "/storage/emulated/0/Android/data/com.gg.bankcardocr/card_number_%d.jpg", i);
//        imwrite(name, numbers[i]);
//    }
//
//
//    gray.release();
//    binary.release();
//    binary_not.release();
//    contours_mat.release();

    return 0;
}

int co1::find_split_cols_pos(const Mat &mat) {
    //处理粘连字符，对中心位置左右的1/4开始扫描，记录最少的黑色像素点的这一列位置，就是粘连字符

    int mid = mat.cols / 2;
    int height = mat.rows;

    //围绕中心左右扫描1/4
    int start_x = mid - mid / 2;

    int end_x = mid + mid / 2;
    //字符的粘连位置
    int cols_pos = mid;
    //获取像素数，0~255

    int c = 0;
    //最小的像素数
    int min_h_p = mat.rows;

    for (int col = start_x; col < end_x; ++col) {
        int total = 0;
        for (int row = 0; row < height; ++row) {
            //获取像素点，opencv 基础第一堂课
            c = mat.at<Vec3b>(row, col)[0];
            //c==0 就是黑色，是黑色的情况下就++；这样就统计出来这一竖列的像素点的个数
            if (c == 0) {
                total++;
            }

        }
        if (total < min_h_p) {
            min_h_p = total;
            cols_pos = col;
        }
    }
    return cols_pos;


}