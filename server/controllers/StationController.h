#pragma once

#include <string>
#include "services/StationService.h"

class StationController {
public:
    explicit StationController(StationService& service);

    void showStationStatus();
    void showLiveStation();
    void searchStation();

private:
    StationService& service_;
};
