#include "StationService.h"
#include "utils/JsonParser.h"
#include "utils/Logger.h"
#include <stdexcept>

StationService::StationService(RailwayApiClient& api) : api_(api) {
}

std::vector<Station> StationService::searchStation(const std::string& query) {
    try {
        auto json = api_.searchStation(query);
        return json_parser::parseStationList(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to search station '" + query +
                                 "': " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to search station '") +
                                 query + "': " + e.what());
    }
}

StationActivity StationService::getTrainsByStation(const std::string& stationCode) {
    try {
        auto json = api_.getTrainsByStation(stationCode);
        return json_parser::parseStationActivity(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get trains at station " +
                                 stationCode + ": " + e.what(),
                                 __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get trains at station ") +
                                 stationCode + ": " + e.what());
    }
}

StationActivity StationService::getLiveStation(const std::string& stationCode) {
    try {
        auto json = api_.getLiveStation(stationCode);
        return json_parser::parseStationActivity(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get live station data for " +
                                 stationCode + ": " + e.what(),
                                 __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get live station data for ") +
                                 stationCode + ": " + e.what());
    }
}