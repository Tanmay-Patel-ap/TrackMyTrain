#include "PNRController.h"
#include "utils/Logger.h"
#include <iostream>
#include <algorithm>
#include <cctype>

PNRController::PNRController(PNRService& service) : service_(service) {}

static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) ++start;
    auto end = s.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;
    return std::string(start, end);
}

void PNRController::showPNRStatus() {
    std::cout << "\n--- PNR Status ---\n";
    std::cout << "Enter PNR Number: ";
    std::string pnr;
    std::getline(std::cin, pnr);
    pnr = trim(pnr);
    if (pnr.empty()) {
        Logger::instance().info("PNR status: empty input",
                                __FILE__, __LINE__);
        return;
    }

    if (pnr.size() != 10 || !std::all_of(pnr.begin(), pnr.end(), ::isdigit)) {
        std::cout << "Invalid PNR number. PNR must be a 10-digit number.\n";
        Logger::instance().warn("Invalid PNR input: '" + pnr + "'",
                                __FILE__, __LINE__);
        return;
    }

    Logger::instance().info("User requested PNR status for " + pnr,
                            __FILE__, __LINE__);
    try {
        auto pnrData = service_.getPNRStatus(pnr);
        std::cout << "\nPNR: " << pnrData.pnrNumber << "\n";
        std::cout << "Train: " << pnrData.trainName << " (" << pnrData.trainNumber << ")\n";
        std::cout << "Date: " << pnrData.boardingDate << "\n";
        std::cout << "From: " << pnrData.from << " -> To: " << pnrData.to << "\n";
        std::cout << "Booking Status: " << pnrData.bookingStatus << "\n";
        std::cout << "\nPassengers:\n";
        for (auto& p : pnrData.passengers) {
            std::cout << "  " << p.name
                      << " | Booking: " << p.bookingStatus
                      << " | Current: " << p.currentStatus
                      << " | Seat: " << p.seatNumber << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        Logger::instance().error("PNR status display error: " +
                                 std::string(e.what()), __FILE__, __LINE__);
    }
}