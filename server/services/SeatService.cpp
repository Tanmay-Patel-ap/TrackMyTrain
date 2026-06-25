#include "SeatService.h"
#include "utils/JsonParser.h"
#include "utils/Logger.h"
#include <stdexcept>

SeatService::SeatService(RailwayApiClient& api) : api_(api) {
}

SeatAvailability SeatService::checkAvailability(const std::string& trainNo,
                                                 const std::string& from,
                                                 const std::string& to,
                                                 const std::string& date,
                                                 const std::string& classCode) {
    try {
        auto json = api_.checkSeatAvailability(trainNo, from, to, date, classCode);
        return json_parser::parseSeatAvailability(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to check seat availability for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to check seat availability for train ") +
                                 trainNo + ": " + e.what());
    }
}

SeatAvailability SeatService::checkAvailabilityV2(const std::string& trainNo,
                                                    const std::string& from,
                                                    const std::string& to,
                                                    const std::string& date,
                                                    const std::string& classCode) {
    try {
        auto json = api_.checkSeatAvailabilityV2(trainNo, from, to, date, classCode);
        return json_parser::parseSeatAvailability(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to check seat availability V2 for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to check seat availability V2 for train ") +
                                 trainNo + ": " + e.what());
    }
}

std::vector<TrainClass> SeatService::getTrainClasses(const std::string& trainNo) {
    try {
        auto json = api_.getTrainClasses(trainNo);
        return json_parser::parseTrainClasses(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get classes for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get classes for train ") +
                                 trainNo + ": " + e.what());
    }
}

Fare SeatService::getFare(const std::string& trainNo,
                           const std::string& from,
                           const std::string& to,
                           const std::string& classCode) {
    try {
        auto json = api_.getFare(trainNo, from, to, classCode);
        return json_parser::parseFare(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get fare for train " +
                                 trainNo + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get fare for train ") +
                                 trainNo + ": " + e.what());
    }
}