#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace log
{
    // 日志级别枚举
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    // 单例 Logger 类
    class Logger
    {
    public:
        // 获取 Logger 实例的静态方法
        static Logger &getInstance()
        {
            static Logger instance;
            return instance;
        }

        // 禁用拷贝构造函数和赋值运算符
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

        // 日志记录方法
        void log(LogLevel level, const std::string &message)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::ofstream logFile("log.txt", std::ios_base::app);
            if (!logFile.is_open())
            {
                throw std::runtime_error("无法打开日志文件");
            }

            // 获取当前时间戳
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::tm buf;
            localtime_s(&buf, &in_time_t);
            std::stringstream ss;
            ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");

            // 写入日志条目
            logFile << ss.str() << " [" << logLevelToString(level) << "] " << message << std::endl;
        }

        // 刷新日志文件
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
        // 私有构造函数
        Logger() = default;

        // 将日志级别转换为字符串
        std::string logLevelToString(LogLevel level)
        {
            switch (level)
            {
            case LogLevel::DEBUG:
                return "DEBUG";
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

        std::mutex mutex_; // 互斥锁
    };
}