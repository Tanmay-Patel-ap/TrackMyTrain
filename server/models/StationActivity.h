#pragma once

#include <string>
#include <vector>
#include "Train.h"

struct StationActivity {
    std::string stationCode;
    std::string stationName;
    std::vector<Train> arrivingTrains;
    std::vector<Train> departingTrains;
};
