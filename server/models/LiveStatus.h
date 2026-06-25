#pragma once

#include <string>

struct LiveStatus {
    std::string trainNumber;
    std::string trainName;
    std::string currentStation;
    std::string delay;
    std::string nextStation;
    std::string lastUpdated;
};
