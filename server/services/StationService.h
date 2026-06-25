#pragma once

#include <string>
#include <vector>
#include "models/Station.h"
#include "models/StationActivity.h"
#include "api/RailwayApiClient.h"

class StationService {
public:
    explicit StationService(RailwayApiClient& api);

    std::vector<Station> searchStation(const std::string& query);
    StationActivity getTrainsByStation(const std::string& stationCode);
    StationActivity getLiveStation(const std::string& stationCode);

private:
    RailwayApiClient& api_;
};
