#pragma once

#include <string>
#include "services/TrainService.h"
#include "services/PNRService.h"
#include "services/StationService.h"
#include "services/SeatService.h"

class Server {
public:
    Server(TrainService& trainSvc, PNRService& pnrSvc,
           StationService& stationSvc, SeatService& seatSvc);
    void start(int port);

private:
    TrainService& trainSvc_;
    PNRService& pnrSvc_;
    StationService& stationSvc_;
    SeatService& seatSvc_;
};
