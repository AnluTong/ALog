//
// Created by andrew on 19-6-4.
//

#ifndef ALOG_MMAPFILEPARSER_H
#define ALOG_MMAPFILEPARSER_H

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

//struct HeadContent {
//    char[4] magic; //ALOG
//    size_t version;
//    size_t len;
//};
class MMapFileParser {
public:
    MMapFileParser(char *ptr, size_t capacity) : size(capacity) {
        assert(capacity > HEAD_LEN);
        initPtr = ptr;
        reset();
    }

    size_t left() {
        return size - (workPtr - initPtr);
    }

    size_t contentLength() {
        size_t curLen = 0;
        memcpy(&curLen, initPtr + LEN_LEN, sizeof(size_t));
        return curLen;
    }

    void write(const char *log, size_t len) {
        assert(len <= left());

        size_t curLen = 0;
        memcpy(&curLen, initPtr + LEN_LEN, sizeof(size_t));
        curLen += len;
        memcpy(initPtr + LEN_LEN, &curLen, sizeof(size_t));

        memcpy(workPtr, log, len);
        workPtr += len;
    }

    void flush(char *dest) {
        memcpy(dest, initPtr + HEAD_LEN, contentLength());
        reset();
    }

    static const char *handleDirtyFile(int fd) {
        off_t len = lseek(fd, 0L, SEEK_END);
        lseek(fd, 0L, SEEK_SET);
        if (len < HEAD_LEN) {
            return NULL;
        }

        char head[HEAD_LEN] = {};
        int ret = 0;
        size_t resultLen = 0;
        ret = read(fd, head, HEAD_LEN);

        bool available = false;
        if (ret == HEAD_LEN) {
            if (head[0] == 'A'
                && head[1] == 'L'
                && head[2] == 'O'
                && head[3] == 'G') {
                memcpy(&resultLen, head + LEN_LEN, sizeof(size_t));
                if (resultLen > 0) {
                    available = true;
                }
            }

        }
        if (!available) {
            return NULL;
        }

        char *content = new char[resultLen];
        read(fd, content, resultLen);
        lseek(fd, 0L, SEEK_SET);
        return content;
    }

private:
    static const size_t HEAD_LEN = 4 * sizeof(char) + sizeof(size_t) + sizeof(size_t);
    static const size_t LEN_LEN = 4 * sizeof(char) + sizeof(size_t);

    const size_t size;
    char *initPtr;
    char *workPtr;

    void reset() {
        memset(initPtr, 0, size);
        workPtr = initPtr + HEAD_LEN;

        char magic[] = {'A', 'L', 'O', 'G'};
        memcpy(initPtr, magic, 4 * sizeof(char));
        size_t version = 0;
        memcpy(initPtr + 4 * sizeof(size_t), &version, sizeof(size_t));
        size_t len = 0;
        memcpy(initPtr + 4 * sizeof(size_t), &len, sizeof(size_t));
    }
};

#endif //ALOG_MMAPFILEPARSER_H
