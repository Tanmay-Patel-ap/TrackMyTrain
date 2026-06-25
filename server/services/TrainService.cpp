#include "TrainService.h"
#include "utils/JsonParser.h"
#include "utils/Logger.h"
#include <stdexcept>

TrainService::TrainService(RailwayApiClient& api) : api_(api) {
}

Train TrainService::getTrainSchedule(const std::string& trainNo) {
    try {
        auto json = api_.getTrainSchedule(trainNo);
        return json_parser::parseTrain(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get schedule for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get schedule for train ") +
                                 trainNo + ": " + e.what());
    }
}

Train TrainService::getTrainScheduleV2(const std::string& trainNo) {
    try {
        auto json = api_.getTrainScheduleV2(trainNo);
        return json_parser::parseTrain(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get schedule V2 for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get schedule V2 for train ") +
                                 trainNo + ": " + e.what());
    }
}

std::vector<ScheduleEntry> TrainService::getScheduleEntries(const std::string& trainNo) {
    try {
        auto json = api_.getTrainSchedule(trainNo);
        return json_parser::parseSchedule(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get route for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get route for train ") +
                                 trainNo + ": " + e.what());
    }
}

LiveStatus TrainService::getLiveStatus(const std::string& trainNo) {
    try {
        auto json = api_.getLiveStatus(trainNo);
        return json_parser::parseLiveStatus(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get live status for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get live status for train ") +
                                 trainNo + ": " + e.what());
    }
}

std::vector<Train> TrainService::searchTrain(const std::string& query) {
    try {
        auto json = api_.searchTrain(query);
        return json_parser::parseTrainList(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to search for '" + query +
                                 "': " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to search for '") +
                                 query + "': " + e.what());
    }
}

Route TrainService::getTrainsBetweenStations(const std::string& from,
                                              const std::string& to,
                                              const std::string& date) {
    try {
        auto json = api_.getTrainsBetweenStations(from, to, date);
        return json_parser::parseRoute(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to find trains from " + from +
                                 " to " + to + " on " + date + ": " + e.what(),
                                 __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to find trains from ") +
                                 from + " to " + to + " on " + date + ": " + e.what());
    }
}