//
// Created by andrew on 19-6-3.
//

#include <string.h>
#include "MMapBuffer.h"

void MMapBuffer::write(const char *log) {
    size_t len = strlen(log);

    size_t left = parse->left();
    if (len < left) {
        parse->write(log, len);
    } else {
        flush();
        if (iWriter) {
            char *data = new char[len];
            memcpy(data, log, len);
            iWriter->write(data);
        }
    }
}

void MMapBuffer::flush() {
    if (iWriter) {
        char *data = new char[parse->contentLength()];
        parse->flush(data);
        iWriter->write(data);
    }
}

MMapBuffer::~MMapBuffer() {
    munmap(initPtr, size);
    if (parse) {
        delete parse;
        parse = NULL;
    }
}

MMapBuffer::MMapBuffer(IWriter *wt, char *bufferPtr, size_t capacity) : IBuffer(wt), size(capacity){
    initPtr = bufferPtr;
    parse = new MMapFileParser(bufferPtr, capacity);
}
