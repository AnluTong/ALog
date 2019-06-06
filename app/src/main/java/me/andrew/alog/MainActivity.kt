package me.andrew.alog

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*
import me.andrew.aloglib.ALog

class MainActivity : AppCompatActivity() {

    private val aLog : ALog by lazy {
        val basePath = getExternalFilesDir(null)
        ALog("$basePath/buffer.mmap", 150 * 1024, "$basePath/a.log")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        hello.setOnClickListener {
            aLog.write("hello world \n")
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        aLog.release()
    }
}
