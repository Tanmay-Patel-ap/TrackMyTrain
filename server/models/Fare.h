#pragma once

#include <string>

struct Fare {
    std::string trainNumber;
    std::string from;
    std::string to;
    std::string className;
    std::string baseFare;
    std::string totalFare;
    std::string distance;
};
