#pragma once

#include <string>
#include <vector>
#include "models/Train.h"
#include "models/ScheduleEntry.h"
#include "models/Station.h"
#include "models/StationActivity.h"
#include "models/LiveStatus.h"
#include "models/PNR.h"
#include "models/Passenger.h"
#include "models/SeatAvailability.h"
#include "models/TrainClass.h"
#include "models/Fare.h"
#include "models/Route.h"

namespace json_parser {

// Train
Train parseTrain(const std::string& json);
std::vector<Train> parseTrainList(const std::string& json);

// Schedule
ScheduleEntry parseScheduleEntry(const std::string& json);
std::vector<ScheduleEntry> parseSchedule(const std::string& json);

// Station
Station parseStation(const std::string& json);
std::vector<Station> parseStationList(const std::string& json);

// Station Activity
StationActivity parseStationActivity(const std::string& json);

// Live Status
LiveStatus parseLiveStatus(const std::string& json);

// PNR
PNR parsePNR(const std::string& json);

// Seat Availability
SeatAvailability parseSeatAvailability(const std::string& json);

// Train Classes
std::vector<TrainClass> parseTrainClasses(const std::string& json);

// Fare
Fare parseFare(const std::string& json);

// Route (trains between stations)
Route parseRoute(const std::string& json);

} // namespace json_parser
