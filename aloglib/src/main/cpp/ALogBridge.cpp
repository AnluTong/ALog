#include <jni.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "DependencyFactory.h"

static const char *sBridgeClassName = "me/andrew/aloglib/ALog";
static IBuffer *buffer;
static IWriter *writer;

static void initALog(JNIEnv *env, jobject instance, jstring jBufferPath, jint jCapacity, jstring jLogPath) {
    const char *bufferPath = env->GetStringUTFChars(jBufferPath, 0);
    const char *logPath = env->GetStringUTFChars(jLogPath, 0);
    size_t capacity = static_cast<size_t>(jCapacity);
    if (buffer) {
        delete buffer;
        buffer = NULL;
    }
    if (writer) {
        delete writer;
        writer = NULL;
    }

    DependencyFactory factory = DependencyFactory();
    writer = factory.getWriter(logPath);

    int fd = open(bufferPath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd != -1) {
        const char *content = MMapFileParser::handleDirtyFile(fd);
        if (content) {
            writer->write(content);
        }
    }

    buffer = factory.getBuffer(fd, capacity, writer);

    env->ReleaseStringUTFChars(jBufferPath, bufferPath);
    env->ReleaseStringUTFChars(jLogPath, logPath);
}

static void writeALog(JNIEnv *env, jobject instance, jstring jLog) {
    if (buffer) {
        const char *log = env->GetStringUTFChars(jLog, 0);
        buffer->write(log);
        env->ReleaseStringUTFChars(jLog, log);
    }
}

static void releaseALog(JNIEnv *env, jobject instance) {
    if (buffer) {
        delete buffer;
        buffer = NULL;
    }
    if (writer) {
        delete writer;
        writer = NULL;
    }
}

static void changeLogPathALog(JNIEnv *env, jobject instance, jstring jLogFilePath) {
    if (buffer) {
        const char *path = env->GetStringUTFChars(jLogFilePath, 0);
        buffer->flush();
        writer->changeLogPath(path);
        env->ReleaseStringUTFChars(jLogFilePath, path);
    }
}

static void flushALog(JNIEnv *env, jobject instance) {
    if (buffer) {
        buffer->flush();
    }
}

static JNINativeMethod gMethods[] = {
        {
                "init",
                "(Ljava/lang/String;ILjava/lang/String;)V",
                (void *) initALog
        },
        {
                "write",
                "(Ljava/lang/String;)V",
                (void *) writeALog
        },
        {
                "flush",
                "()V",
                (void *) flushALog
        },
        {
                "changeLogPath",
                "(Ljava/lang/String;)V",
                (void *) changeLogPathALog
        },
        {
                "release",
                "()V",
                (void *) releaseALog
        }
};

#ifdef __cplusplus
extern "C"
#endif

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_FALSE;
    }
    jclass classDocScanner = env->FindClass(sBridgeClassName);
    if (env->RegisterNatives(classDocScanner, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return;
    }
}

#ifdef __cplusplus
#endif