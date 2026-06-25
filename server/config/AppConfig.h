#pragma once

#include <string>

enum class ApiMode { REAL, MOCK };

class AppConfig {
public:
    static AppConfig& instance();

    std::string baseUrl() const { return "https://irctc1.p.rapidapi.com"; }
    std::string apiKey() const { return apiKey_; }
    std::string apiHost() const { return "irctc1.p.rapidapi.com"; }
    ApiMode mode() const { return mode_; }

    void setApiKey(const std::string& key);
    void setMode(ApiMode mode) { mode_ = mode; }

private:
    AppConfig();
    std::string apiKey_;
    ApiMode mode_;
};
