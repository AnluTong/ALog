//
// Created by andrew on 19-6-4.
//

#include "FileWriter.h"

void FileWriter::write(const char *log) {
    taskQueue.push_back(log);
    taskCondition.notify_all();
}

void FileWriter::changeLogPath(const char *path) {
    if (file) {
        fclose(file);
        file = NULL;
    }
    file = fopen(path, "ab+");
}

FileWriter::~FileWriter() {
    exit = true;
    taskCondition.notify_all();
    taskThread.join();

    if (file) {
        fclose(file);
        file = NULL;
    }
}

FileWriter::FileWriter(const char *path) {
    file = fopen(path, "ab+");
    taskThread = std::thread(&FileWriter::handleTask, this);
}

void FileWriter::handleTask() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!taskQueue.empty()) {
            const char *log = taskQueue.back();
            taskQueue.pop_back();
            size_t len = strlen(log);
            if (file && len > 0) {
                fwrite(log, 1, len, file);
                fflush(file);
            }
            delete[] log;
        }
        if (exit) {
            return;
        }
        taskCondition.wait(lock);
    }
}
