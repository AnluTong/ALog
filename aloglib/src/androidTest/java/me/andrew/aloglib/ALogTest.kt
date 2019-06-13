package me.andrew.aloglib

import android.support.test.InstrumentationRegistry
import android.support.test.runner.AndroidJUnit4
import android.util.Log
import org.junit.After
import org.junit.Test

import org.junit.Assert.*
import org.junit.Before
import org.junit.runner.RunWith

/**
 * created by andrew.tong
 * on 19-6-12.
 */
@RunWith(AndroidJUnit4::class)
class ALogTest {

    private lateinit var aLog: ALog

    @Before
    fun setUp() {
        val appContext = InstrumentationRegistry.getTargetContext()
        val basePath = appContext.getExternalFilesDir(null)
        aLog = ALog("$basePath/buffer.mmap", 150 * 1024, "$basePath/a.log")
    }

    @After
    fun release() {
        aLog.flush()
    }

    @Test
    fun write() {
        var now = System.currentTimeMillis()
        for (i in 0 until 1000) {
            aLog.write("hello world \n")
        }
        Log.d("ALog", "alog use time ${System.currentTimeMillis() - now}")

        now = System.currentTimeMillis()
        for (i in 0 until 1000) {
            Log.d("tst", "hello world")
        }
        Log.d("ALog", "tst use time ${System.currentTimeMillis() - now}")

        Thread.sleep(1000)
    }
}