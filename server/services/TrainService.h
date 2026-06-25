#pragma once

#include <string>
#include <vector>
#include "models/Train.h"
#include "models/ScheduleEntry.h"
#include "models/LiveStatus.h"
#include "models/Route.h"
#include "api/RailwayApiClient.h"

class TrainService {
public:
    explicit TrainService(RailwayApiClient& api);

    Train getTrainSchedule(const std::string& trainNo);
    Train getTrainScheduleV2(const std::string& trainNo);
    std::vector<ScheduleEntry> getScheduleEntries(const std::string& trainNo);

    LiveStatus getLiveStatus(const std::string& trainNo);

    std::vector<Train> searchTrain(const std::string& query);

    Route getTrainsBetweenStations(const std::string& from,
                                    const std::string& to,
                                    const std::string& date);

private:
    RailwayApiClient& api_;
};
