//
// Created by andrew on 19-6-4.
//

#ifndef ALOG_DEPENDENCYFACTORY_H
#define ALOG_DEPENDENCYFACTORY_H

#include "IBuffer.h"
#include "MMapBuffer.h"
#include "MemoryBuffer.h"
#include "IWriter.h"
#include "FileWriter.h"

class DependencyFactory {
public:
    IBuffer *getBuffer(int fd, size_t capacity, IWriter *writer) {
        char *bufferPtr = NULL;
        if (fd != -1) {
            //adjust size
            ftruncate(fd, static_cast<off_t>(capacity));
            lseek(fd, 0, SEEK_SET);
            bufferPtr = static_cast<char *>(mmap(0, capacity, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0));
            if (bufferPtr == MAP_FAILED) {
                bufferPtr = NULL;
            }
        }

        if (bufferPtr == NULL) {
            return new MemoryBuffer(writer, new char[capacity], capacity);
        } else {
            return new MMapBuffer(writer, bufferPtr, capacity);
        }
    }

    IWriter *getWriter(const char *logPath) {
        return new FileWriter(logPath);
    }
};

#endif //ALOG_DEPENDENCYFACTORY_H
