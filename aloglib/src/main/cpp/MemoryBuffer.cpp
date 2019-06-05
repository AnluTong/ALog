//
// Created by andrew on 19-6-3.
//

#include <string.h>
#include "MemoryBuffer.h"

void MemoryBuffer::write(const char *log) {
    size_t len = strlen(log);

    size_t left = size - (workPtr - initPtr);
    if (len < left) {
        memcpy(workPtr, log, len);
        workPtr += len;
    } else {
        flush();
        if (iWriter) {
            char *data = new char[len];
            memcpy(data, log, len);
            iWriter->write(data);
        }
    }
}

void MemoryBuffer::flush() {
    if (iWriter) {
        char *data = new char[size];
        memcpy(data, initPtr, size);
        memset((void *) initPtr, 0, size);
        iWriter->write(data);
        workPtr -= (workPtr - initPtr);
    }
}

MemoryBuffer::~MemoryBuffer() {
    if (initPtr) {
        delete[] initPtr;
        initPtr = NULL;
    }
}

MemoryBuffer::MemoryBuffer(IWriter *wt, char *bufferPtr, size_t capacity) : IBuffer(wt), size(capacity) {
    initPtr = bufferPtr;
    workPtr = bufferPtr;
}
