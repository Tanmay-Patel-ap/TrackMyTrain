#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

Logger::Logger()
    : level_(LogLevel::INFO)
    , quiet_(false) {
    file_.open("trackmytrain.log", std::ios::app);
    if (file_.is_open()) {
        file_ << "\n=== Session started at " << currentTimestamp() << " ===\n";
    }
}

Logger::~Logger() {
    if (file_.is_open()) {
        file_ << "=== Session ended at " << currentTimestamp() << " ===\n";
        file_.close();
    }
}

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    level_ = level;
    info("Log level set to " + levelToString(level));
}

void Logger::setQuiet(bool quiet) {
    std::lock_guard<std::mutex> lock(mutex_);
    quiet_ = quiet;
}

void Logger::log(LogLevel level, const std::string& message,
                 const std::string& file, int line) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (level < level_) return;

    std::string timestamp = currentTimestamp();
    std::string levelStr = levelToString(level);

    std::string location;
    if (!file.empty()) {
        size_t pos = file.find_last_of("/\\");
        std::string fname = (pos != std::string::npos) ? file.substr(pos + 1) : file;
        location = " [" + fname;
        if (line > 0) location += ":" + std::to_string(line);
        location += "]";
    }

    std::string logLine = "[" + timestamp + "] [" + levelStr + "]" + location + " " + message;

    if (!quiet_) {
        std::cout << logLine << std::endl;
    }
    if (file_.is_open()) {
        file_ << logLine << std::endl;
    }
}

void Logger::debug(const std::string& message,
                   const std::string& file, int line) {
    log(LogLevel::DEBUG, message, file, line);
}

void Logger::info(const std::string& message,
                  const std::string& file, int line) {
    log(LogLevel::INFO, message, file, line);
}

void Logger::warn(const std::string& message,
                  const std::string& file, int line) {
    log(LogLevel::WARN, message, file, line);
}

void Logger::error(const std::string& message,
                   const std::string& file, int line) {
    log(LogLevel::ERROR, message, file, line);
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:              return "UNKN";
    }
}

std::string Logger::currentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
