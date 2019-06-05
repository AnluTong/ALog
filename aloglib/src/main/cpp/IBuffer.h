//
// Created by andrew on 19-6-3.
//

#ifndef ALOG_IBUFFER_H
#define ALOG_IBUFFER_H


#include "IWriter.h"

class IBuffer {
public:
    virtual void write(const char *log) = 0;
    virtual void flush() = 0;

protected:
    IWriter *iWriter;

public:
    IBuffer(IWriter *wt) : iWriter(wt) {}
    virtual ~IBuffer() {}
};


#endif //ALOG_IBUFFER_H
