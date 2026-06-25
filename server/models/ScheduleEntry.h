#pragma once

#include <string>

struct ScheduleEntry {
    std::string stationCode;
    std::string stationName;
    std::string arrivalTime;
    std::string departureTime;
    std::string distance;
    int day;
};
