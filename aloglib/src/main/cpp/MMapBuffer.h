//
// Created by andrew on 19-6-3.
//

#ifndef ALOG_MMAPBUFFER_H
#define ALOG_MMAPBUFFER_H

#include <sys/mman.h>
#include "IBuffer.h"
#include "MMapFileParser.h"

class MMapBuffer : public IBuffer {
public:
    MMapBuffer(IWriter *wt, char *bufferPtr, size_t capacity);
    virtual ~MMapBuffer();
    virtual void write(const char *log);
    virtual void flush();

private:
    const size_t size;
    char *initPtr;
    MMapFileParser *parse;
};


#endif //ALOG_MMAPBUFFER_H
