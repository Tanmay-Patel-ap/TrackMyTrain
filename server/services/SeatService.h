#pragma once

#include <string>
#include <vector>
#include "models/SeatAvailability.h"
#include "models/TrainClass.h"
#include "models/Fare.h"
#include "api/RailwayApiClient.h"

class SeatService {
public:
    explicit SeatService(RailwayApiClient& api);

    SeatAvailability checkAvailability(const std::string& trainNo,
                                        const std::string& from,
                                        const std::string& to,
                                        const std::string& date,
                                        const std::string& classCode);
    SeatAvailability checkAvailabilityV2(const std::string& trainNo,
                                          const std::string& from,
                                          const std::string& to,
                                          const std::string& date,
                                          const std::string& classCode);
    std::vector<TrainClass> getTrainClasses(const std::string& trainNo);
    Fare getFare(const std::string& trainNo,
                 const std::string& from,
                 const std::string& to,
                 const std::string& classCode);

private:
    RailwayApiClient& api_;
};
