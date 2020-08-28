//
// Created by GG on 2020/8/26.
//

#ifndef BANKCARDOCR_CARDOCR_H
#define BANKCARDOCR_CARDOCR_H

#include "opencv2/opencv.hpp"

using namespace cv;

namespace co1 {

/**
 * 找到银行卡区域
 * @param mat  图片的mat
 * @param area  卡号区域
 * @return 是否成功 0成功 其他失败
 */
    int find_card_ocr(const Mat &mat, Rect &area);
}


namespace co2 {

}


#endif //BANKCARDOCR_CARDOCR_H
