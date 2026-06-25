#pragma once

#include <string>
#include <vector>
#include "Train.h"
#include "ScheduleEntry.h"

struct Route {
    Train train;
    std::vector<ScheduleEntry> stations;
    std::string totalDistance;
    std::string duration;
};
