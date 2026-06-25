#include "JsonParser.h"
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

namespace json_parser {

static json parseAndValidate(const std::string& jsonStr) {
    try {
        auto j = json::parse(jsonStr);
        return j;
    } catch (const json::parse_error& e) {
        throw std::runtime_error(std::string("Invalid JSON response: ") + e.what());
    }
}

static void requireData(const json& j) {
    if (!j.contains("data") || j["data"].is_null()) {
        std::string msg = j.value("message", "Unknown error");
        throw std::runtime_error("API error: " + msg);
    }
}

static Train parseTrainFromJson(const json& j) {
    Train t;
    t.trainNumber = j.value("train_number", "");
    t.trainName = j.value("train_name", "");
    t.source = j.value("source", "");
    t.destination = j.value("destination", "");
    t.type = j.value("type", "");
    return t;
}

static ScheduleEntry parseScheduleFromJson(const json& j) {
    ScheduleEntry e;
    e.stationCode = j.value("station_code", "");
    e.stationName = j.value("station_name", "");
    e.arrivalTime = j.value("arrival", "");
    e.departureTime = j.value("departure", "");
    e.distance = j.value("distance", "");
    e.day = j.value("day", 1);
    return e;
}

static Passenger parsePassengerFromJson(const json& j) {
    Passenger p;
    p.name = j.value("name", "");
    p.bookingStatus = j.value("booking_status", "");
    p.currentStatus = j.value("current_status", "");
    p.seatNumber = j.value("seat_number", 0);
    return p;
}

Train parseTrain(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    return parseTrainFromJson(j["data"]);
}

std::vector<Train> parseTrainList(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    if (!data.is_array()) {
        throw std::runtime_error("Expected array in response data");
    }
    std::vector<Train> trains;
    for (auto& item : data) {
        trains.push_back(parseTrainFromJson(item));
    }
    return trains;
}

std::vector<ScheduleEntry> parseSchedule(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    if (!data.contains("schedule") || !data["schedule"].is_array()) {
        throw std::runtime_error("No schedule data found in response");
    }
    std::vector<ScheduleEntry> entries;
    for (auto& item : data["schedule"]) {
        entries.push_back(parseScheduleFromJson(item));
    }
    return entries;
}

Station parseStation(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    Station s;
    s.stationCode = data.value("station_code", "");
    s.stationName = data.value("station_name", "");
    return s;
}

std::vector<Station> parseStationList(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    if (!data.is_array()) {
        throw std::runtime_error("Expected array in response data");
    }
    std::vector<Station> stations;
    for (auto& item : data) {
        Station s;
        s.stationCode = item.value("station_code", "");
        s.stationName = item.value("station_name", "");
        stations.push_back(s);
    }
    return stations;
}

StationActivity parseStationActivity(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    StationActivity sa;
    sa.stationCode = data.value("station_code", "");
    sa.stationName = data.value("station_name", "");

    if (data.contains("arriving") && data["arriving"].is_array()) {
        for (auto& t : data["arriving"]) {
            sa.arrivingTrains.push_back(parseTrainFromJson(t));
        }
    }
    if (data.contains("departing") && data["departing"].is_array()) {
        for (auto& t : data["departing"]) {
            sa.departingTrains.push_back(parseTrainFromJson(t));
        }
    }
    return sa;
}

LiveStatus parseLiveStatus(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    LiveStatus ls;
    ls.trainNumber = data.value("train_number", "");
    ls.trainName = data.value("train_name", "");
    ls.currentStation = data.value("current_station_name", "");
    ls.delay = data.value("delay", "");
    ls.nextStation = data.value("next_station_name", "");
    ls.lastUpdated = data.value("last_updated", "");
    return ls;
}

PNR parsePNR(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    PNR pnr;
    pnr.pnrNumber = data.value("pnr", "");
    pnr.trainNumber = data.value("train_number", "");
    pnr.trainName = data.value("train_name", "");
    pnr.boardingDate = data.value("boarding_date", "");
    pnr.from = data.value("from", "");
    pnr.to = data.value("to", "");
    pnr.bookingStatus = data.value("booking_status", "");

    if (data.contains("passengers") && data["passengers"].is_array()) {
        for (auto& p : data["passengers"]) {
            pnr.passengers.push_back(parsePassengerFromJson(p));
        }
    }
    return pnr;
}

SeatAvailability parseSeatAvailability(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    SeatAvailability sa;
    sa.trainNumber = data.value("train_number", "");
    sa.trainName = data.value("train_name", "");
    sa.date = data.value("date", "");
    sa.from = data.value("from", "");
    sa.to = data.value("to", "");
    sa.className = data.value("class", "");
    sa.availableSeats = data.value("available_seats", "");
    sa.fare = data.value("fare", "");
    return sa;
}

std::vector<TrainClass> parseTrainClasses(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    if (!data.is_array()) {
        throw std::runtime_error("Expected array in response data");
    }
    std::vector<TrainClass> classes;
    for (auto& item : data) {
        TrainClass tc;
        tc.classCode = item.value("class_code", "");
        tc.className = item.value("class_name", "");
        tc.fare = item.value("fare", "");
        classes.push_back(tc);
    }
    return classes;
}

Fare parseFare(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    Fare f;
    f.trainNumber = data.value("train_number", "");
    f.from = data.value("from", "");
    f.to = data.value("to", "");
    f.className = data.value("class", "");
    f.baseFare = data.value("base_fare", "");
    f.totalFare = data.value("total_fare", "");
    f.distance = data.value("distance", "");
    return f;
}

Route parseRoute(const std::string& jsonStr) {
    auto j = parseAndValidate(jsonStr);
    requireData(j);
    auto& data = j["data"];
    Route route;
    if (!data.is_array() || data.empty()) {
        return route;
    }
    route.train = parseTrainFromJson(data[0]);
    route.totalDistance = data[0].value("distance", "");
    route.duration = data[0].value("duration", "");

    if (data[0].contains("stations") && data[0]["stations"].is_array()) {
        for (auto& s : data[0]["stations"]) {
            route.stations.push_back(parseScheduleFromJson(s));
        }
    }
    return route;
}

} // namespace json_parser