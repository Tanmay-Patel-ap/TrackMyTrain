#include "SeatController.h"
#include "utils/Logger.h"
#include <iostream>
#include <algorithm>
#include <cctype>

SeatController::SeatController(SeatService& service) : service_(service) {}

static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) ++start;
    auto end = s.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;
    return std::string(start, end);
}

void SeatController::checkSeatAvailability() {
    std::cout << "\n--- Seat Availability ---\n";
    std::cout << "Train Number: ";
    std::string trainNo;
    std::getline(std::cin, trainNo);
    trainNo = trim(trainNo);
    if (trainNo.empty()) {
        Logger::instance().info("Seat availability: empty train number",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "From (Code): ";
    std::string from;
    std::getline(std::cin, from);
    from = trim(from);
    if (from.empty()) {
        Logger::instance().info("Seat availability: empty 'from'",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "To (Code): ";
    std::string to;
    std::getline(std::cin, to);
    to = trim(to);
    if (to.empty()) {
        Logger::instance().info("Seat availability: empty 'to'",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "Date (YYYY-MM-DD): ";
    std::string date;
    std::getline(std::cin, date);
    date = trim(date);
    if (date.empty()) {
        Logger::instance().info("Seat availability: empty date",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "Class (e.g. 3A, SL, 2A): ";
    std::string cls;
    std::getline(std::cin, cls);
    cls = trim(cls);
    if (cls.empty()) {
        Logger::instance().info("Seat availability: empty class",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User checking seat availability for train " +
                            trainNo + " " + from + "->" + to + " " + date +
                            " class " + cls, __FILE__, __LINE__);
    try {
        auto avail = service_.checkAvailability(trainNo, from, to, date, cls);
        std::cout << "\nTrain: " << avail.trainName << " (" << avail.trainNumber << ")\n";
        std::cout << "From: " << avail.from << " -> " << avail.to << "\n";
        std::cout << "Date: " << avail.date << " | Class: " << avail.className << "\n";
        std::cout << "Available Seats: " << avail.availableSeats << "\n";
        std::cout << "Fare: Rs. " << avail.fare << "\n";

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Seat availability display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void SeatController::showTrainClasses() {
    std::cout << "\n--- Train Classes ---\n";
    std::cout << "Enter Train Number: ";
    std::string trainNo;
    std::getline(std::cin, trainNo);
    trainNo = trim(trainNo);
    if (trainNo.empty()) {
        Logger::instance().info("Train classes: empty input",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested classes for train " + trainNo,
                            __FILE__, __LINE__);
    try {
        auto classes = service_.getTrainClasses(trainNo);
        if (classes.empty()) {
            std::cout << "No classes found.\n";
            return;
        }
        std::cout << "\nClasses for Train " << trainNo << ":\n";
        for (auto& c : classes) {
            std::cout << "  " << c.classCode << " | " << c.className
                      << " | Rs. " << c.fare << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Train classes display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}

void SeatController::showFare() {
    std::cout << "\n--- Fare Enquiry ---\n";
    std::cout << "Train Number: ";
    std::string trainNo;
    std::getline(std::cin, trainNo);
    trainNo = trim(trainNo);
    if (trainNo.empty()) {
        Logger::instance().info("Fare enquiry: empty train number",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "From (Code): ";
    std::string from;
    std::getline(std::cin, from);
    from = trim(from);
    if (from.empty()) {
        Logger::instance().info("Fare enquiry: empty 'from'",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "To (Code): ";
    std::string to;
    std::getline(std::cin, to);
    to = trim(to);
    if (to.empty()) {
        Logger::instance().info("Fare enquiry: empty 'to'",
                                __FILE__, __LINE__);
        return;
    }

    std::cout << "Class (e.g. 3A, SL, 2A): ";
    std::string cls;
    std::getline(std::cin, cls);
    cls = trim(cls);
    if (cls.empty()) {
        Logger::instance().info("Fare enquiry: empty class",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested fare for train " + trainNo +
                            " " + from + "->" + to + " class " + cls,
                            __FILE__, __LINE__);
    try {
        auto fare = service_.getFare(trainNo, from, to, cls);
        std::cout << "\nFare Details:\n";
        std::cout << "Train: " << fare.trainNumber << "\n";
        std::cout << "Route: " << fare.from << " -> " << fare.to << "\n";
        std::cout << "Class: " << fare.className << "\n";
        std::cout << "Base Fare: Rs. " << fare.baseFare << "\n";
        std::cout << "Total Fare: Rs. " << fare.totalFare << "\n";
        std::cout << "Distance: " << fare.distance << " km\n";

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("Fare display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}