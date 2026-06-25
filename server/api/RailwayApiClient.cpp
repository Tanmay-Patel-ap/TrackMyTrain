#include "RailwayApiClient.h"
#include "config/AppConfig.h"
#include "utils/Logger.h"
#include <stdexcept>

RailwayApiClient::RailwayApiClient()
    : client_(AppConfig::instance().baseUrl()) {
}

std::string RailwayApiClient::mockPath(const std::string& filename) const {
    return std::string(TEST_DATA_DIR) + "/" + filename;
}

std::string RailwayApiClient::callApi(const std::string& endpoint,
                                       const std::string& mockFile) {
    auto& config = AppConfig::instance();

    if (config.mode() == ApiMode::MOCK) {
        if (mockFile.empty()) {
            Logger::instance().error("Mock filename cannot be empty",
                                     __FILE__, __LINE__);
            throw std::invalid_argument("Mock filename cannot be empty");
        }
        return HttpClient::readMockFile(mockPath(mockFile));
    }

    auto resp = client_.get(endpoint, config.apiKey(), config.apiHost());
    if (resp.error) {
        Logger::instance().error("API call failed: " + resp.errorMessage,
                                 __FILE__, __LINE__);
        throw std::runtime_error("API request failed: " + resp.errorMessage);
    }
    if (resp.body.empty()) {
        Logger::instance().error("API returned empty response for " + endpoint,
                                __FILE__, __LINE__);
        throw std::runtime_error("API returned empty response");
    }
    return resp.body;
}

static void requireNonEmpty(const std::string& val, const std::string& name) {
    if (val.empty()) {
        Logger::instance().error("Parameter '" + name + "' is empty",
                                 __FILE__, __LINE__);
        throw std::invalid_argument(name + " cannot be empty");
    }
}

std::string RailwayApiClient::getTrainSchedule(const std::string& trainNo) {
    requireNonEmpty(trainNo, "Train number");
    return callApi("/api/v1/getTrainSchedule?trainNo=" + trainNo,
                   "train_schedule.json");
}

std::string RailwayApiClient::getTrainScheduleV2(const std::string& trainNo) {
    requireNonEmpty(trainNo, "Train number");
    return callApi("/api/v2/getTrainSchedule?trainNo=" + trainNo,
                   "train_schedule_v2.json");
}

std::string RailwayApiClient::getPNRStatus(const std::string& pnr) {
    requireNonEmpty(pnr, "PNR number");
    return callApi("/api/v3/getPNRStatus?pnr=" + pnr,
                   "pnr_status.json");
}

std::string RailwayApiClient::getLiveStatus(const std::string& trainNo) {
    requireNonEmpty(trainNo, "Train number");
    return callApi("/api/v1/getLiveTrainStatus?trainNo=" + trainNo,
                   "live_status.json");
}

std::string RailwayApiClient::searchStation(const std::string& query) {
    requireNonEmpty(query, "Search query");
    return callApi("/api/v1/searchStation?query=" + query,
                   "search_station.json");
}

std::string RailwayApiClient::getTrainsByStation(const std::string& stationCode) {
    requireNonEmpty(stationCode, "Station code");
    return callApi("/api/v1/getTrainsByStation?stationCode=" + stationCode,
                   "trains_by_station.json");
}

std::string RailwayApiClient::getLiveStation(const std::string& stationCode) {
    requireNonEmpty(stationCode, "Station code");
    return callApi("/api/v1/getLiveStation?stationCode=" + stationCode,
                   "live_station.json");
}

std::string RailwayApiClient::searchTrain(const std::string& query) {
    requireNonEmpty(query, "Search query");
    return callApi("/api/v1/searchTrain?query=" + query,
                   "search_train.json");
}

std::string RailwayApiClient::getTrainsBetweenStations(const std::string& from,
                                                        const std::string& to,
                                                        const std::string& date) {
    requireNonEmpty(from, "Source station code");
    requireNonEmpty(to, "Destination station code");
    requireNonEmpty(date, "Date");
    return callApi("/api/v3/trainsBetweenStations?from=" + from +
                   "&to=" + to + "&date=" + date,
                   "trains_between_stations.json");
}

std::string RailwayApiClient::checkSeatAvailability(const std::string& trainNo,
                                                     const std::string& from,
                                                     const std::string& to,
                                                     const std::string& date,
                                                     const std::string& classCode) {
    requireNonEmpty(trainNo, "Train number");
    requireNonEmpty(from, "Source station code");
    requireNonEmpty(to, "Destination station code");
    requireNonEmpty(date, "Date");
    requireNonEmpty(classCode, "Class code");
    return callApi("/api/v1/checkSeatAvailability?trainNo=" + trainNo +
                   "&from=" + from + "&to=" + to +
                   "&date=" + date + "&class=" + classCode,
                   "seat_availability.json");
}

std::string RailwayApiClient::checkSeatAvailabilityV2(const std::string& trainNo,
                                                       const std::string& from,
                                                       const std::string& to,
                                                       const std::string& date,
                                                       const std::string& classCode) {
    requireNonEmpty(trainNo, "Train number");
    requireNonEmpty(from, "Source station code");
    requireNonEmpty(to, "Destination station code");
    requireNonEmpty(date, "Date");
    requireNonEmpty(classCode, "Class code");
    return callApi("/api/v2/checkSeatAvailability?trainNo=" + trainNo +
                   "&from=" + from + "&to=" + to +
                   "&date=" + date + "&class=" + classCode,
                   "seat_availability_v2.json");
}

std::string RailwayApiClient::getTrainClasses(const std::string& trainNo) {
    requireNonEmpty(trainNo, "Train number");
    return callApi("/api/v1/getTrainClasses?trainNo=" + trainNo,
                   "train_classes.json");
}

std::string RailwayApiClient::getFare(const std::string& trainNo,
                                       const std::string& from,
                                       const std::string& to,
                                       const std::string& classCode) {
    requireNonEmpty(trainNo, "Train number");
    requireNonEmpty(from, "Source station code");
    requireNonEmpty(to, "Destination station code");
    requireNonEmpty(classCode, "Class code");
    return callApi("/api/v1/getFare?trainNo=" + trainNo +
                   "&from=" + from + "&to=" + to +
                   "&class=" + classCode,
                   "fare.json");
}