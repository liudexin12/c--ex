#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

class Logger
{
public:
    // 日志级别
    enum class LogLevel
    {
        INFO,
        WARNING,
        ERROR
    };

    // 获取 Logger 实例
    static Logger &getInstance()
    {
        static Logger instance;
        return instance;
    }

    // 禁用拷贝构造函数和赋值操作符
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    // 记录消息
    void log(LogLevel level, const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream logFile("log.txt", std::ios_base::app);
        if (logFile.is_open())
        {
            logFile << getCurrentTime() << " [" << logLevelToString(level) << "] " << message << std::endl;
        }
    }

    // 刷新日志
    void flush()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream logFile("log.txt", std::ios_base::app);
        if (logFile.is_open())
        {
            logFile.flush();
        }
    }

private:
    Logger() = default;
    ~Logger() = default;

    // 获取当前时间
    std::string getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }

    // 将日志级别转换为字符串
    std::string logLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    std::mutex mutex_;
};

#endif