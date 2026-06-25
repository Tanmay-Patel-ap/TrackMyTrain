#include <iostream>
#include <string>
#include <limits>
#include "config/AppConfig.h"
#include "api/RailwayApiClient.h"
#include "services/TrainService.h"
#include "services/PNRService.h"
#include "services/StationService.h"
#include "services/SeatService.h"
#include "controllers/TrainController.h"
#include "controllers/PNRController.h"
#include "controllers/StationController.h"
#include "controllers/SeatController.h"
#include "utils/Logger.h"
#include "Server.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printHeader() {
    std::cout << "\n========================================\n";
    std::cout << "        TRACKMYTRAIN - RailCore\n";
    std::cout << "========================================\n";
}

void printMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << " 1. Train Schedule\n";
    std::cout << " 2. PNR Status\n";
    std::cout << " 3. Live Train Status\n";
    std::cout << " 4. Station Status\n";
    std::cout << " 5. Search Train\n";
    std::cout << " 6. Trains Between Stations\n";
    std::cout << " 7. Seat Availability\n";
    std::cout << " 8. Fare Enquiry\n";
    std::cout << " 0. Exit\n";
    std::cout << "Choice: ";
}

static bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    bool serverMode = false;
    int port = 8080;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--server") {
            serverMode = true;
            if (i + 1 < argc) {
                port = std::stoi(argv[++i]);
            }
        }
    }

    auto& config = AppConfig::instance();
    auto& log = Logger::instance();
    log.setQuiet(true);

    RailwayApiClient apiClient;
    TrainService trainSvc(apiClient);
    PNRService pnrSvc(apiClient);
    StationService stationSvc(apiClient);
    SeatService seatSvc(apiClient);

    if (serverMode) {
        std::cout << "Enter RapidAPI Key (or press Enter to use MOCK mode): ";
        std::string key;
        std::getline(std::cin, key);
        config.setApiKey(key);

        if (config.mode() == ApiMode::REAL) {
            std::cout << "Mode set to REAL API\n";
        } else {
            std::cout << "Mode set to MOCK (using local test data)\n";
        }

        Server server(trainSvc, pnrSvc, stationSvc, seatSvc);
        server.start(port);
        return 0;
    }

    std::cout << "Enter RapidAPI Key (or press Enter to use MOCK mode): ";
    std::string key;
    std::getline(std::cin, key);
    config.setApiKey(key);

    if (config.mode() == ApiMode::REAL) {
        std::cout << "Mode set to REAL API\n";
    } else {
        std::cout << "Mode set to MOCK (using local test data)\n";
    }

    TrainController trainCtrl(trainSvc);
    PNRController pnrCtrl(pnrSvc);
    StationController stationCtrl(stationSvc);
    SeatController seatCtrl(seatSvc);

    int choice;
    do {
        printHeader();
        printMenu();

        std::string input;
        std::getline(std::cin, input);

        if (!isNumber(input)) {
            std::cout << "\nInvalid input. Please enter a number.\n";
            log.warn("Invalid menu input: '" + input + "'",
                     __FILE__, __LINE__);
            choice = -1;
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            continue;
        }

        choice = std::stoi(input);

        switch (choice) {
            case 1:
                log.info("Menu option 1 selected: Train Schedule",
                         __FILE__, __LINE__);
                trainCtrl.showTrainSchedule();
                break;
            case 2:
                log.info("Menu option 2 selected: PNR Status",
                         __FILE__, __LINE__);
                pnrCtrl.showPNRStatus();
                break;
            case 3:
                log.info("Menu option 3 selected: Live Train Status",
                         __FILE__, __LINE__);
                trainCtrl.showLiveStatus();
                break;
            case 4:
                log.info("Menu option 4 selected: Station Status",
                         __FILE__, __LINE__);
                stationCtrl.showStationStatus();
                break;
            case 5:
                log.info("Menu option 5 selected: Search Train",
                         __FILE__, __LINE__);
                trainCtrl.searchTrain();
                break;
            case 6:
                log.info("Menu option 6 selected: Trains Between Stations",
                         __FILE__, __LINE__);
                trainCtrl.showTrainsBetweenStations();
                break;
            case 7:
                log.info("Menu option 7 selected: Seat Availability",
                         __FILE__, __LINE__);
                seatCtrl.checkSeatAvailability();
                break;
            case 8:
                log.info("Menu option 8 selected: Fare Enquiry",
                         __FILE__, __LINE__);
                seatCtrl.showFare();
                break;
            case 0:
                log.info("Menu option 0 selected: Exit",
                         __FILE__, __LINE__);
                std::cout << "\nGoodbye!\n";
                break;
            default:
                log.warn("Invalid menu choice: " + std::to_string(choice),
                         __FILE__, __LINE__);
                std::cout << "\nInvalid choice.\n";
                break;
        }
        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (choice != 0);

    return 0;
}