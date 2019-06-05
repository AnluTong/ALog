//
// Created by andrew on 19-6-4.
//

#ifndef ALOG_IWRITER_H
#define ALOG_IWRITER_H

class IWriter {
public:
    virtual void write(const char *log) = 0;
    virtual void changeLogPath(const char *path) = 0;
    virtual ~IWriter() {}
};
#endif //ALOG_IWRITER_H
