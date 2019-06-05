//
// Created by andrew on 19-6-4.
//

#ifndef ALOG_FILEWRITER_H
#define ALOG_FILEWRITER_H


#include "IWriter.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class FileWriter : public IWriter {
public:
    FileWriter(const char *path);
    virtual void write(const char *log);
    virtual void changeLogPath(const char *path);
    virtual ~FileWriter();

private:
    void handleTask();
    bool exit = false;
    FILE *file;

    std::vector<const char*> taskQueue;
    std::thread taskThread;
    std::condition_variable taskCondition;
    std::mutex mtx;
};


#endif //ALOG_FILEWRITER_H
