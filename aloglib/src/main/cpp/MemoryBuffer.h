//
// Created by andrew on 19-6-3.
//

#ifndef ALOG_MEMORYBUFFER_H
#define ALOG_MEMORYBUFFER_H

#include <stdint.h>
#include "IBuffer.h"

class MemoryBuffer : public IBuffer {
public:
    MemoryBuffer(IWriter *wt, char *bufferPtr, size_t capacity);
    virtual ~MemoryBuffer();
    virtual void write(const char *log);
    virtual void flush();

private:
    const size_t size;
    char *initPtr;
    char *workPtr;
};


#endif //ALOG_MEMORYBUFFER_H
