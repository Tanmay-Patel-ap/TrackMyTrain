#include "AppConfig.h"
#include "utils/Logger.h"

AppConfig& AppConfig::instance() {
    static AppConfig config;
    return config;
}

AppConfig::AppConfig()
    : apiKey_("YOUR_RAPIDAPI_KEY_HERE")
    , mode_(ApiMode::MOCK) {
}

void AppConfig::setApiKey(const std::string& key) {
    if (key.empty()) {
        mode_ = ApiMode::MOCK;
        apiKey_ = "";
        Logger::instance().info("Mode set to MOCK (no API key provided)",
                                __FILE__, __LINE__);
        return;
    }
    apiKey_ = key;
    mode_ = ApiMode::REAL;
    Logger::instance().info("Mode set to REAL API",
                            __FILE__, __LINE__);
}