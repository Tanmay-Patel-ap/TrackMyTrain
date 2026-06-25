#pragma once

#include <string>
#include "HttpClient.h"

class RailwayApiClient {
public:
    RailwayApiClient();

    // Train Schedule
    std::string getTrainSchedule(const std::string& trainNo);
    std::string getTrainScheduleV2(const std::string& trainNo);

    // PNR Status
    std::string getPNRStatus(const std::string& pnr);

    // Live Train Status
    std::string getLiveStatus(const std::string& trainNo);

    // Station
    std::string searchStation(const std::string& query);
    std::string getTrainsByStation(const std::string& stationCode);
    std::string getLiveStation(const std::string& stationCode);

    // Search
    std::string searchTrain(const std::string& query);

    // Routes
    std::string getTrainsBetweenStations(const std::string& from,
                                          const std::string& to,
                                          const std::string& date);

    // Seat & Fare
    std::string checkSeatAvailability(const std::string& trainNo,
                                       const std::string& from,
                                       const std::string& to,
                                       const std::string& date,
                                       const std::string& classCode);
    std::string checkSeatAvailabilityV2(const std::string& trainNo,
                                         const std::string& from,
                                         const std::string& to,
                                         const std::string& date,
                                         const std::string& classCode);
    std::string getTrainClasses(const std::string& trainNo);
    std::string getFare(const std::string& trainNo,
                        const std::string& from,
                        const std::string& to,
                        const std::string& classCode);

private:
    std::string mockPath(const std::string& filename) const;
    std::string callApi(const std::string& endpoint, const std::string& mockFile);

    HttpClient client_;
};
