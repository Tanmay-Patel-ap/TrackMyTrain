#pragma once

#include <string>

struct SeatAvailability {
    std::string trainNumber;
    std::string trainName;
    std::string date;
    std::string from;
    std::string to;
    std::string className;
    std::string availableSeats;
    std::string fare;
};
