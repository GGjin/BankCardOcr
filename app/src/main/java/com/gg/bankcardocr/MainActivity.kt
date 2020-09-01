package com.gg.bankcardocr

import android.content.Context
import android.graphics.BitmapFactory
import android.media.MediaScannerConnection
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.io.InputStream

class MainActivity : AppCompatActivity() {


    val Tag = "card---ocr"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method


        val mCardBitmap = BitmapFactory.decodeResource(resources, R.drawable.card)

        externalCacheDir!!.absolutePath + File.separator

        moveApkFromAssets(this, "card_n.png")

        sample_text.setOnClickListener {
            val cardNumber = BankCardOcr.carOcr(mCardBitmap)

            Log.w(Tag, cardNumber)
        }

    }

    fun getAppPath(context: Context, name: String) =
        context.externalCacheDir!!.absolutePath + File.separator + name


    fun moveApkFromAssets(context: Context, name: String): Boolean {
        try {
            val path: String = getAppPath(context, name)
            val mFile = File(path)
            if (mFile.exists()) {
                return true
            }
            val mInputStream: InputStream = context.assets.open(name)
            if (!mFile.exists()) mFile.createNewFile()
            val mFileOutputStream = FileOutputStream(mFile)
            val mbyte = ByteArray(1024)
            var i = 0
            while (mInputStream.read(mbyte).also { i = it } > 0) {
                mFileOutputStream.write(mbyte, 0, i)
            }
            mInputStream.close()
            mFileOutputStream.close()
            MediaScannerConnection.scanFile(context, arrayOf(mFile.absolutePath), null, null)
        } catch (e: IOException) {
            e.printStackTrace()
            return false
        } catch (e: Exception) {
            e.printStackTrace()
            return false
        }
        return true
    }

}
