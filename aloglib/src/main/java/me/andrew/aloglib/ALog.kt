@file:JvmName("ALog")

package me.andrew.aloglib

/**
 * created by andrew.tong
 * on 19-6-3.
 */
class ALog constructor(bufferPath: String, bufferCapacity: Int, logPath: String) {

    init {
        init(bufferPath, bufferCapacity, logPath)
    }

    private external fun init(bufferPath: String, bufferCapacity: Int, logPath: String)
    external fun write(log: String)
    external fun flush()
    external fun release()
    external fun changeLogPath(logPath: String)

    companion object {
        init {
            System.loadLibrary("alog")
        }
    }
}