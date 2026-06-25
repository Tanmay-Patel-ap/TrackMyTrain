#include "StationController.h"
#include "utils/Logger.h"
#include <iostream>
#include <algorithm>
#include <cctype>

StationController::StationController(StationService& service) : service_(service) {}

static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) ++start;
    auto end = s.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;
    return std::string(start, end);
}

void StationController::showStationStatus() {
    std::cout << "\n--- Station Status ---\n";
    std::cout << "Enter Station Code: ";
    std::string code;
    std::getline(std::cin, code);
    code = trim(code);
    if (code.empty()) {
        Logger::instance().info("Station status: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested station status for " + code,
                            __FILE__, __LINE__);
    try {
        auto activity = service_.getTrainsByStation(code);
        std::cout << "\nStation: " << activity.stationName << " (" << activity.stationCode << ")\n";

        std::cout << "\nArriving Trains:\n";
        if (activity.arrivingTrains.empty()) {
            std::cout << "  None\n";
        } else {
            for (auto& t : activity.arrivingTrains) {
                std::cout << "  " << t.trainNumber << " | " << t.trainName << "\n";
            }
        }

        std::cout << "\nDeparting Trains:\n";
        if (activity.departingTrains.empty()) {
            std::cout << "  None\n";
        } else {
            for (auto& t : activity.departingTrains) {
                std::cout << "  " << t.trainNumber << " | " << t.trainName << "\n";
            }
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Station status display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void StationController::showLiveStation() {
    std::cout << "\n--- Live Station ---\n";
    std::cout << "Enter Station Code: ";
    std::string code;
    std::getline(std::cin, code);
    code = trim(code);
    if (code.empty()) {
        Logger::instance().info("Live station: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested live station for " + code,
                            __FILE__, __LINE__);
    try {
        auto activity = service_.getLiveStation(code);
        std::cout << "\nLive Station: " << activity.stationName << " (" << activity.stationCode << ")\n";
        std::cout << "\nTrains at Station:\n";
        if (activity.arrivingTrains.empty() && activity.departingTrains.empty()) {
            std::cout << "  No trains at station.\n";
        } else {
            for (auto& t : activity.arrivingTrains) {
                std::cout << "  " << t.trainNumber << " | " << t.trainName << "\n";
            }
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Live station display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void StationController::searchStation() {
    std::cout << "\n--- Search Station ---\n";
    std::cout << "Enter Station Name/Code: ";
    std::string query;
    std::getline(std::cin, query);
    query = trim(query);
    if (query.empty()) {
        Logger::instance().info("Search station: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User searched station for '" + query + "'",
                            __FILE__, __LINE__);
    try {
        auto stations = service_.searchStation(query);
        if (stations.empty()) {
            std::cout << "No stations found.\n";
            return;
        }
        std::cout << "\nFound " << stations.size() << " station(s):\n";
        for (auto& s : stations) {
            std::cout << "  " << s.stationCode << " | " << s.stationName << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Search station display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}