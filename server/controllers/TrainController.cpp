#include "TrainController.h"
#include "utils/Logger.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

TrainController::TrainController(TrainService& service) : service_(service) {}

static bool isBlank(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); });
}

static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) ++start;
    auto end = s.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;
    return std::string(start, end);
}

void TrainController::showTrainSchedule() {
    std::cout << "\n--- Train Schedule ---\n";
    std::cout << "Enter Train Number: ";
    std::string trainNo;
    std::getline(std::cin, trainNo);
    trainNo = trim(trainNo);
    if (trainNo.empty()) {
        Logger::instance().info("Train schedule: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested schedule for train " + trainNo,
                            __FILE__, __LINE__);
    try {
        auto train = service_.getTrainSchedule(trainNo);
        auto entries = service_.getScheduleEntries(trainNo);

        std::cout << "\nTrain: " << train.trainName << " (" << train.trainNumber << ")\n";
        std::cout << "Type: " << train.type << "\n";
        std::cout << "Source: " << train.source << " -> Destination: " << train.destination << "\n";
        std::cout << "\nRoute:\n";
        std::cout << std::left << std::setw(8) << "Station" << std::setw(28) << "Name"
                  << std::setw(10) << "Arrival" << std::setw(12) << "Departure"
                  << std::setw(10) << "Dist(km)" << "Day\n";
        std::cout << std::string(75, '-') << "\n";

        for (auto& e : entries) {
            std::cout << std::setw(8) << e.stationCode << std::setw(28) << e.stationName
                      << std::setw(10) << e.arrivalTime << std::setw(12) << e.departureTime
                      << std::setw(10) << e.distance << e.day << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Train schedule display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void TrainController::showLiveStatus() {
    std::cout << "\n--- Live Train Status ---\n";
    std::cout << "Enter Train Number: ";
    std::string trainNo;
    std::getline(std::cin, trainNo);
    trainNo = trim(trainNo);
    if (trainNo.empty()) {
        Logger::instance().info("Live status: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested live status for train " + trainNo,
                            __FILE__, __LINE__);
    try {
        auto status = service_.getLiveStatus(trainNo);
        std::cout << "\nTrain: " << status.trainName << " (" << status.trainNumber << ")\n";
        std::cout << "Current Station: " << status.currentStation << "\n";
        std::cout << "Next Station: " << status.nextStation << "\n";
        std::cout << "Delay: " << status.delay << "\n";
        std::cout << "Last Updated: " << status.lastUpdated << "\n";

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Live status display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void TrainController::searchTrain() {
    std::cout << "\n--- Search Train ---\n";
    std::cout << "Enter Train Name/Number: ";
    std::string query;
    std::getline(std::cin, query);
    query = trim(query);
    if (query.empty()) {
        Logger::instance().info("Search train: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User searched train for '" + query + "'",
                            __FILE__, __LINE__);
    try {
        auto trains = service_.searchTrain(query);
        if (trains.empty()) {
            std::cout << "No trains found.\n";
            return;
        }
        std::cout << "\nFound " << trains.size() << " train(s):\n";
        for (auto& t : trains) {
            std::cout << t.trainNumber << " | " << t.trainName
                      << " | " << t.source << " -> " << t.destination
                      << " | " << t.type << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Search train display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void TrainController::showTrainsBetweenStations() {
    std::cout << "\n--- Trains Between Stations ---\n";
    std::cout << "From (Station Code): ";
    std::string from;
    std::getline(std::cin, from);
    from = trim(from);
    if (from.empty()) {
        Logger::instance().info("Trains between: empty 'from' input",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "To (Station Code): ";
    std::string to;
    std::getline(std::cin, to);
    to = trim(to);
    if (to.empty()) {
        Logger::instance().info("Trains between: empty 'to' input",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "Date (DD-MM-YYYY): ";
    std::string date;
    std::getline(std::cin, date);
    date = trim(date);
    if (date.empty()) {
        Logger::instance().info("Trains between: empty date input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested trains from " + from + " to " + to +
                            " on " + date, __FILE__, __LINE__);
    try {
        auto route = service_.getTrainsBetweenStations(from, to, date);
        if (route.train.trainNumber.empty()) {
            std::cout << "No trains found.\n";
            return;
        }
        std::cout << "\nTrain: " << route.train.trainName << " (" << route.train.trainNumber << ")\n";
        std::cout << "Duration: " << route.duration << " | Distance: " << route.totalDistance << " km\n";
        std::cout << "\nRoute:\n";
        for (auto& s : route.stations) {
            std::cout << s.stationCode << " | " << s.stationName
                      << " | Arr: " << s.arrivalTime << " | Dep: " << s.departureTime << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Trains between display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}