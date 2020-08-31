//
// Created by GG on 2020/8/26.
//

#ifndef BANKCARDOCR_CARDOCR_H
#define BANKCARDOCR_CARDOCR_H

#include "opencv2/opencv.hpp"
#include "vector"

using namespace cv;
using namespace std;

namespace co1 {

    /**
     * 找到银行卡区域
     * @param mat  图片的mat
     * @param area  卡号区域
     * @return 是否成功 0成功 其他失败
     */
    int find_card_ocr(const Mat &mat, Rect &area);

    /**
     * 裁剪银行卡号区域
     * @param mat  银行卡的mat
     * @param area  存放截取区域
     * @return  是否成功
     */
    int find_card_number_area(const Mat &mat, Rect &area);

    /**
     * 找到所有数字
     * @param mat  银行卡号区域
     * @param numbers 存放所有数字
     * @return 是否成功
     */
    int find_card_numbers(const Mat &mat, vector<Mat> numbers);

    /**
     * 判断粘连字符
     * @param mat
     * @return
     */
    int find_split_cols_pos(const Mat &mat);
}


namespace co2 {

}


#endif //BANKCARDOCR_CARDOCR_H
