cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(src/main/cpp/ DIR_LIB_SOURCE)
include_directories(src/main/cpp/include)

add_library(
        alog
        SHARED
        ${DIR_LIB_SOURCE})
find_library(
        log
        android
        alog)
target_link_libraries(
        alog
        ${log-lib})