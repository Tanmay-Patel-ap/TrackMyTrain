#pragma once

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    LOG_ERROR
};

class Logger {
public:
    static Logger& instance();

    void setLevel(LogLevel level);
    void setQuiet(bool quiet);

    void log(LogLevel level, const std::string& message,
             const std::string& file = "", int line = 0);

    void debug(const std::string& message,
               const std::string& file = "", int line = 0);
    void info(const std::string& message,
              const std::string& file = "", int line = 0);
    void warn(const std::string& message,
              const std::string& file = "", int line = 0);
    void error(const std::string& message,
               const std::string& file = "", int line = 0);

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream file_;
    LogLevel level_;
    bool quiet_;
    std::mutex mutex_;

    static std::string levelToString(LogLevel level);
    static std::string currentTimestamp();
};
