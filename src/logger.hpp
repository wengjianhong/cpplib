/***********************************************************
 * @file        logger.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       异步日志记录器
 * @version     0.1
 * @date        2023-10-26
 * @copyright   Copyright (c) 2023
 ***********************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include<stdio.h>
#include<stdint.h>

#include<queue>
#include<string>
#include<thread>

class Logger
{
private:
    time_t                      interval_ = 5;          // 间隔刷新时间, 单位: 秒, 默认5秒
    size_t                      filesize_ = 128 << 20;  // 文件最大大小, 单位: B, 默认128MB
    std::string                 name_ = "";             // 日志记录器名称
    std::queue<std::string>     buffer_;                // 日志缓冲区

public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

public:
    Logger(std::string name);
    ~Logger();


    void Flush();


    // Getter / Setter
    void SetName(const std::string& name) { name_ = name; }
    std::string GetName() { return name_; }

    void SetInterval(time_t interval) { interval_ = interval; }
    time_t GetInterval() { return interval_; }

    void SetFileSize(size_t filesize) { filesize_ = filesize; }
    size_t GetFileSize() { return filesize_; }
};


#define LOG_TRACE if (muduo::Logger::logLevel() <= muduo::Logger::TRACE) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (muduo::Logger::logLevel() <= muduo::Logger::DEBUG) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (muduo::Logger::logLevel() <= muduo::Logger::INFO) \
  muduo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()
#define LOG_ERROR muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()
#define LOG_FATAL muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()
#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()



#endif // !__LOGGER_H__
