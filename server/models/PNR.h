#pragma once

#include <string>
#include <vector>
#include "Passenger.h"

struct PNR {
    std::string pnrNumber;
    std::string trainNumber;
    std::string trainName;
    std::string boardingDate;
    std::string from;
    std::string to;
    std::string bookingStatus;
    std::vector<Passenger> passengers;
};
