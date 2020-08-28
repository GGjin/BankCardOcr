package com.gg.bankcardocr

import android.graphics.Bitmap

/**
 *  Create by GG on 2020/8/28
 *  mail is gg.jin.yu@gmail.com
 */
object BankCardOcr {


    init {
        System.loadLibrary("native-lib")
    }

    external fun carOcr(bitmap: Bitmap): String

}