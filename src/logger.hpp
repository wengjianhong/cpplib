/***********************************************************
 * @file        logger.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       Implementation of asynchronous logger
 * @version     0.1
 * @date        2023-10-26
 * @copyright   Copyright (c) 2023
 ***********************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include<time.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>

#include<string>
#include<thread>
#include<chrono>

#include "safe_queue.hpp"

class Logger
{
    const static size_t QUEUE_SIZE = 51200;     // Maximum number of queue logs.
    const static size_t BUFFER_SIZE = 1 << 15;  // Size of formate buffer.

public:
    enum class LOG_LEVEL                        // Log level
    {
        DEBUG = 10,                             // Debugging information
        INFO = 20,                              // General information
        WARN = 30,                              // Dangerous but not wrong
        ERROR = 40,                             // Unexpected behavior / Incorrect state
        CRITICAL = 50,                          // Extremely important
        FATAL = 60,                             // Program cannot proceed
    };

    enum class THREAD_STATUS                    // thread status
    {
        INIT = 0,
        PAUSE = 1,
        RUNNING = 2,
        ERROR = 3,
        EXIT = 4,
    };

private:
    int                     initFlag_;          // Whether it has been initialized.
    FILE*                   logFile_;           // log file.
    char*                   formateBuffer_;     // format buffer.

    std::string             loggerName_;        // logger name.
    std::string             savePath_;          // Path for saving log files.

    size_t                  logFileSize_;       // The maximum size of a file. (unit: B)
    time_t                  flushInterval_;     // Interval refresh time. (unit: B)
    time_t                  currentTimestamp_;  // Current timestamp

    LOG_LEVEL               logLevel_;          // default log level.
    THREAD_STATUS           threadStatus_;      // thread status.

    std::thread*            lpThread_;
    SafeQueue<std::string>  messageQueue_;      // safe queue of log message

public:
    /* ============= Constructor / Destructor ============== */
    Logger(const std::string& name, const std::string& path);
    ~Logger();

    /* ================== Getter / Setter ================== */
    void SetLogLevel(LOG_LEVEL level) { logLevel_ = level; }
    LOG_LEVEL GetLogLevel() { return logLevel_; }

    void SetFileSize(size_t filesize) { logFileSize_ = filesize; }
    size_t GetFileSize() { return logFileSize_; }

    void SetName(const std::string& name) { loggerName_ = name; }
    std::string GetName() { return loggerName_; }

    void SetSavePath(const std::string& path) { savePath_ = path; }
    std::string GetSavePath() { return savePath_; }

    void SetInterval(time_t interval) { flushInterval_ = interval; }
    time_t GetInterval() { return flushInterval_; }

    /**
     * @brief           Log a message on the logger.
     * @tparam ...Args
     * @param level     Log message severity level
     * @param file      file name
     * @param line      line number
     * @param formate   C string that contains a format string that follows the same
                        specifications as format in printf (see printf for details).
     * @param ...args   Depending on the format string, the function may expect a
                        sequence of additional arguments, each containing a value to be
                        used to replace a format specifier in the format string
                        (or a pointer to a storage location, for n).
    */
    template <typename... Args>
    void Log(LOG_LEVEL level, const char* file, size_t line, const char* formate,
             Args&&... args);

    /**
     * @brief           If DEBUG is defined, a debug message is logged on the logger.
                        Or do nothing.
     * @tparam ...Args
     * @param file      file name.
     * @param line      line number
     * @param formate   C string that contains a format string that follows the same
                        specifications as format in printf (see printf for details).
     * @param ...args   Depending on the format string, the function may expect a
                        sequence of additional arguments, each containing a value to be
                        used to replace a format specifier in the format string
                        (or a pointer to a storage location, for n).
    */
    template <typename... Args>
    void DLog(const char* file, size_t line, const char* formate, Args&&... args);

    /**
     * @brief
    */
    void Flush();

    /**
     * @brief       Initialize variables and allocate space.
     * @return      0 indicates success, others indicate error.
    */
    int  Init();

private:
    /**
     * @brief
    */
    void ThreadProcess();

    /**
     * @brief
    */
    void SwitchNewFile();

    /**
     * @brief           Get log description.
     * @param level     log level.
     * @return          log level description string.
    */
    static const char* GetLevelInfo(Logger::LOG_LEVEL level);
};

Logger::Logger(const std::string& name, const std::string& path)
    : loggerName_(name),  savePath_(path)
{
    initFlag_ = 0;
    lpThread_ = nullptr;
    formateBuffer_ = nullptr;
    currentTimestamp_ = time(0);

    logFile_ = nullptr;
    flushInterval_ = 5;
    logFileSize_ = 128 << 20;
    logLevel_ = LOG_LEVEL::INFO;
    threadStatus_ = THREAD_STATUS::INIT;

    while (messageQueue_.size() > 0) {
        messageQueue_.pop();
    }
}

Logger::~Logger()
{
    threadStatus_ = THREAD_STATUS::EXIT;

    if (lpThread_ != nullptr) {
        lpThread_->join();
    }

    if(logFile_ != nullptr){
        (void)fclose(logFile_);
    }
}

template <typename... Args>
void Logger::Log(LOG_LEVEL level, const char* file, size_t line, const char* format,
                 Args&&... args)
{
    if (initFlag_ == 0 && Init() != 0) {
        return;
    }

    // formate time
    (void)strftime(formateBuffer_, BUFFER_SIZE,
                   "[%Y-%m-%d %H:%M:%S] ", std::localtime(&currentTimestamp_));

    // formate locatione
    size_t offset = strlen(formateBuffer_);
    (void)snprintf(formateBuffer_ + offset, BUFFER_SIZE - offset,
                   "%20s:%-8lu", file, line);

    // formate level
    offset = strlen(formateBuffer_);
    (void)snprintf(formateBuffer_ + offset, BUFFER_SIZE - offset,
                   "[%s] ", GetLevelInfo(level));

    // formate message
    offset = strlen(formateBuffer_);
    (void)snprintf(formateBuffer_ + offset, BUFFER_SIZE - offset,
                   format, std::forward<Args>(args)...);

    messageQueue_.emplace((std::string(formateBuffer_) + std::string("\n\r")));
}

template<typename ...Args>
void Logger::DLog(const char* file, size_t line, const char* formate, Args && ...args)
{
#ifdef DEBUG
    Log(LOG_LEVEL::DEBUG, func, line, formate, std::forward<Args>(args)...);
#endif // DEBUG
}

int Logger::Init()
{
    formateBuffer_ = new char[BUFFER_SIZE];
    if (formateBuffer_ == nullptr) {
        return ENOMEM;      // Cannot allocate memory
    }

    lpThread_ = new std::thread(&Logger::ThreadProcess, this);
    if (lpThread_ == nullptr) {
        delete[] formateBuffer_;
        return EFAULT;      // Bad address
    }

    SwitchNewFile();
    initFlag_ = 1;
    return 0;
}

void Logger::Flush()
{
    if (initFlag_ == 0 && Init() != 0)
    {
        return;
    }

    size_t fileSize = 0;
    while (logFile_ != nullptr && messageQueue_.empty() == false) {
        // Write the message to a file and do nothing if it fails
        std::string& message = messageQueue_.front();
        (void)fputs(message.c_str(), logFile_);
        messageQueue_.pop();

        // Check the file size and switch to a new file if the file is larger than size.
        fileSize = fseek(logFile_, 0, SEEK_END);
        if (fileSize >= logFileSize_) {
            SwitchNewFile();
        }
    }
}

void Logger::ThreadProcess()
{
    time_t lastFlushTimestamp = 0;

    while (threadStatus_ == THREAD_STATUS::RUNNING) {
        currentTimestamp_ = time(nullptr);

        // Execute Flush() every flushInterval_ seconds
        if (currentTimestamp_ - lastFlushTimestamp >= flushInterval_) {
            Flush();
        }

        // Execute Flush() if queue message reach 25% of QUEUE_SIZE.
        if (messageQueue_.size() >= QUEUE_SIZE >> 2) {
            Flush();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    Flush();
}

void Logger::SwitchNewFile()
{
    // close old log file.
    if (logFile_ != nullptr) {
        (void)fclose(logFile_);
        logFile_ = nullptr;
    }

    // formate log file name.
    char fileName[256] = { 0 };
    (void)snprintf(fileName, sizeof(fileName), "%s/%s_", savePath_.c_str(), loggerName_.c_str());
    (void)strftime(fileName + strlen(fileName), sizeof(fileName) - strlen(fileName),
        "%Y-%m-%d_%H-%M-%S.log", std::localtime(&currentTimestamp_));

    // open new log file.
    logFile_ = fopen(fileName, "w");
}

const char* Logger::GetLevelInfo(Logger::LOG_LEVEL level){
    switch(level){
        case LOG_LEVEL::DEBUG:
            return "DEBUG";
        case LOG_LEVEL::INFO:
            return "INFO";
        case LOG_LEVEL::WARN:
            return "WARN";
        case LOG_LEVEL::ERROR:
            return "ERROR";
        case LOG_LEVEL::CRITICAL:
            return "CRITICAL";
        case LOG_LEVEL::FATAL:
            return "FATAL";
        default:
            return "INFO";
    }
}

#endif // !__LOGGER_H__
